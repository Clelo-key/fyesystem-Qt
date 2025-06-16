#include "myRegister.h"

#include <passwordEncoder/passwordEncoder.h>
#include <regexValidate/regexValidate.h>
#include <sqlHelper/sqlHelper.h>

#include <optional>

MyRegister::MyRegister(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::MyRegister),
      messageBox(new MyMessageBox(this)) {
  ui->setupUi(this);

  connect(this->ui->loginButton, &MyLabel::PressSignals, this,
          &MyRegister::toLoginPage);
}

MyRegister::~MyRegister() {
  delete messageBox;
  delete ui;
}

bool MyRegister::validateDatabase(const QString& username, const QString& phone,
                                  const QString& password,
                                  const QString& nickname) {
  try {
    QSqlDatabase conn = SqlHelper::GetInstance().getDatabase();
    if (!conn.isOpen()) {
      qCritical() << "数据库连接失败";
      return (false);
    }

    QSqlQuery query(conn);

    // 验证用户名是否存在
    query.prepare("SELECT 1 FROM sys_user WHERE username = ? LIMIT 1;");
    query.addBindValue(username);
    if (!query.exec()) {
      qCritical() << "SQL查询执行失败:" << query.lastError().text();
      return (false);
    }

    if (query.next()) {
      messageBox->showMessage("用户已存在", MyMessageBox::INFO);
      return (false);
    }

    // 验证手机号是否冲突
    query.prepare("SELECT 1 FROM sys_user WHERE phone = ? LIMIT 1;");
    query.addBindValue(phone);
    if (!query.exec()) {
      qCritical() << "SQL查询执行失败:" << query.lastError().text();
      return (false);
    }

    if (query.next()) {
      messageBox->showMessage("已查询到改手机号已被其它用户绑定",
                              MyMessageBox::INFO);
      return (false);
    }

    // 插入信息
    bool preparesuccess = query.prepare(
        "INSERT INTO sys_user (username, phone, password,nickname) "
        "VALUES (:username, :phone, :password,:nickname);");
    if (!preparesuccess) {
      qCritical() << "SQL查询执行失败:" << query.lastError().text();
    }
    std::string encodePassword =
        PasswordEncoder::encode(password.toStdString());

    query.bindValue(":username", username);
    query.bindValue(":phone", phone);
    query.bindValue(":password", QString::fromStdString(encodePassword));
    query.bindValue(":nickname", nickname);

    // 执行插入
    if (query.exec()) {
      messageBox->showMessage("用户注册成功", MyMessageBox::INFO);
      return (true);
    } else {
      qCritical() << "SQL查询执行失败:" << query.lastError().text();
      messageBox->showMessage("系统错误😥", MyMessageBox::INFO);
    }
  } catch (const std::exception& e) {
    qCritical() << "系统异常:" << e.what();
    return (false);
  }
  return (false);
}

bool MyRegister::validateCredentials(std::optional<QString>& account,
                                     std::optional<QString>& phone,
                                     std::optional<QString>& password,
                                     std::optional<QString>& password2,
                                     std::optional<QString>& nickname) {
  RegexValidate validator;

  if (!nickname || nickname->trimmed().isEmpty()) {
    nickname = "FyeUser";
  }

  if (!account || account->trimmed().isEmpty()) {
    messageBox->showMessage("用户名不能为空", MyMessageBox::ERROR);

    return (false);
  }

  if (!phone || phone->trimmed().isEmpty()) {
    messageBox->showMessage("手机号不能为空", MyMessageBox::WARNING);

    return (false);
  }

  if (!password || password->isEmpty()) {
    messageBox->showMessage("密码不能为空", MyMessageBox::SUCCESS);
    return (false);
  }

  if (!password2 || password2->isEmpty()) {
    messageBox->showMessage("请确认密码", MyMessageBox::INFO);
    return (false);
  }

  // 2. 格式验证
  // 验证手机号格式
  if (!validator.validatePhone(phone.value().toStdString())) {
    messageBox->showMessage("请输入中国大陆合法手机号", MyMessageBox::INFO);
    return (false);
  }

  // 验证用户名格式
  if (!validator.validateUsername(account.value().toStdString())) {
    messageBox->showMessage("用户名只能包含字母、数字和下划线，长度4-20位",
                            MyMessageBox::INFO);
    return (false);
  }

  // validator.setInput(password.value().toStdString());
  // if (!validator.setPattern(RegexValidate::Pattern::Password).validate()) {
  //   new MyMessageBox(this, "Fye system",
  //                    "密码必须包含大小写字母、数字和特殊字符中的至少两种");
  //   return;
  // }

  // 3. 一致性检查
  if (password.value() != password2.value()) {
    messageBox->showMessage("密码输入不一致", MyMessageBox::INFO);
    return (false);
  }

  return (true);
}

void MyRegister::on_registerButton_clicked() {
  std::optional<QString> account = ui->usrLineEdit->text();
  std::optional<QString> phone = ui->phoneEdit->text();
  std::optional<QString> password = ui->pwdLineEdit->text();
  std::optional<QString> password2 = ui->pwdLineEdit2->text();
  std::optional<QString> nickname = ui->nicknameLineEdit->text();

  if (!validateCredentials(account, phone, password, password2, nickname)) {
    return;
  }
  try {
    QString pwd = password.value();
    bool success =
        validateDatabase(account.value(), phone.value(), pwd, nickname.value());
    pwd.clear();
    if (success) {
      emit registerSuccess(account.value(), password.value());
    }
  } catch (const std::exception& e) {
    qCritical() << "注册过程发生错误:" << e.what();
    messageBox->showMessage("注册失败，请稍后重试", MyMessageBox::ERROR);
  }
}
