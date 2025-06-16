#include "myLoginWindow.h"

MyLoginWindow::MyLoginWindow(QWidget *parent)
    : QMainWindow(parent), messagebox(new MyMessageBox(this)) {
  ui.setupUi(this);
  // 弹出到顶层
  // setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
  //
  // QFile qssFile(":asset/style/myMenu.qss");
  // if (qssFile.open(QFile::ReadOnly)) {
  //   this->setStyleSheet(qssFile.readAll());
  // }
  // qssFile.close();
  // this->setAttribute(Qt::WA_DeleteOnClose, true);
  initSingnals();
}

MyLoginWindow::~MyLoginWindow() {}

void MyLoginWindow::initSingnals() {
  // 跳转到register页面
  connect(ui.loginPage, &MyLogin::toRegisterPage, this,
          [&]() { ui.stackedWidget->setCurrentIndex(1); });
  // 登录成功，关闭当前页
  connect(ui.loginPage, &MyLogin::loginSuccess, this, [&]() {
    this->sureQuit = true;
    emit loginComplate();
    this->close();
  });
  // 跳转到login页面
  connect(ui.registerPage, &MyRegister::toLoginPage, this,
          [&]() { ui.stackedWidget->setCurrentIndex(0); });
  // 注册成功跳转到login页面
  connect(ui.registerPage, &MyRegister::registerSuccess, this,
          [&]() { ui.stackedWidget->setCurrentIndex(0); });
  // 注册成功跳转，自动填写login页面数据
  connect(ui.registerPage, &MyRegister::registerSuccess, ui.loginPage,
          &MyLogin::fillAccountInfo);

  connect(messagebox, &MyMessageBox::cancle, this,
          [&]() { this->sureQuit = false; });

  connect(messagebox, &MyMessageBox::makeSure, this, [&]() {
    this->sureQuit = true;
    this->close();  // 不需用户再次确认
  });
}

void MyLoginWindow::closeEvent(QCloseEvent *event) {
  event->ignore();
  if (!this->sureQuit) {
    messagebox->showMessage("确认退出吗?", MyMessageBox::WARNING, 0);
    // emit finished();   // 用户取消登录,退出应用
  } else {
    QMainWindow::closeEvent(event);
    this->disconnect();
    this->deleteLater();
  }
}
