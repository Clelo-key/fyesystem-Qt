#include "myLogin.h"

#include <passwordEncoder/passwordEncoder.h>
#include <sqlHelper/sqlHelper.h>

MyLogin::MyLogin(QWidget *parent)
    : QWidget(parent), ui(new Ui::MyLogin), messageBox(new MyMessageBox(this)) {
  ui->setupUi(this);

  connect(this->ui->registerLabel, &MyLabel::PressSignals, this,
          &MyLogin::toRegisterPage);
}

MyLogin::~MyLogin() {
  delete ui;
  delete messageBox;
}

bool MyLogin::validateDatabase(const QString &username,
                               const QString &password) {
  try {
    QSqlDatabase conn = SqlHelper::GetInstance().getDatabase();
    if (!conn.isOpen()) {
      qCritical() << "数据库连接失败";
      return (false);
    }

    QSqlQuery query(conn);
    query.prepare("SELECT password FROM sys_user WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec()) {
      qCritical() << "SQL查询执行失败:" << query.lastError().text();
      return (false);
    }

    // 用户不存在
    if (!query.next()) {
      return (false);
    }

    // 验证密码
    return (PasswordEncoder::matches(password.toStdString(),
                                     query.value(0).toString().toStdString()));
  } catch (const std::exception &e) {
    qCritical() << "系统异常:" << e.what();
    return (false);
  }
}

void MyLogin::on_loginButton_clicked() {
  std::optional<QString> account = ui->usrLineEdit->text();
  std::optional<QString> password = ui->pwdLineEdit->text();

  // 空值检查
  if (!account || account->trimmed().isEmpty()) {
    messageBox->showMessage("用户名不能为空", MyMessageBox::INFO);
    return;
  }

  if (!password || password->isEmpty()) {
    messageBox->showMessage("密码不能为空", MyMessageBox::INFO);
    return;
  }

  try {
    // 清空内存，防止密码泄漏
    QString pwd = password.value();
    bool success = validateDatabase(account.value(), pwd);
    pwd.clear();
    if (success) {
      emit loginSuccess();
    } else {
      messageBox->showMessage("账号或密码有误😥", MyMessageBox::INFO);
    }
  } catch (const std::exception &e) {
    qCritical() << "登录过程发生错误:" << e.what();
    messageBox->showMessage("登录过程发生错误，请稍后重试", MyMessageBox::INFO);
  }
}

void MyLogin::fillAccountInfo(const QString &username,
                              const QString &password) {
  this->ui->usrLineEdit->setText(username);
  this->ui->pwdLineEdit->setText(password);
}
