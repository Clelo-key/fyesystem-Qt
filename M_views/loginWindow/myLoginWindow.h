#pragma once

#include <myMessageBox/MyMessageBox.h>

#include <QFile>
#include <QImageReader>
#include <QMainWindow>

#include "ui_myLoginWindow.h"

class MyLoginWindow : public QMainWindow {
  Q_OBJECT

 public:
  MyLoginWindow(QWidget *parent = nullptr);

  ~MyLoginWindow();

  void initSingnals();

 signals:
  void loginComplate();
  void finished();  // 用户取消登录

 private:
  void closeEvent(QCloseEvent *event) override;

 private:
  Ui::MyLoginWindow ui;
  MyMessageBox *messagebox;
  bool sureQuit = false;
};
