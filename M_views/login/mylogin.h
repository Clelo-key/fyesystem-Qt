#pragma once


#include <QWidget>
#include <myMessageBox/MyMessageBox.h>

#include "ui_mylogin.h"

class MyLogin : public QWidget {
   Q_OBJECT

public:
   MyLogin(QWidget *parent = nullptr);

   ~MyLogin();

private:
   Ui::MyLogin *ui;
   MyMessageBox *messageBox;

   bool validateDatabase(const QString&username, const QString&password);

private slots:
   void on_loginButton_clicked();

public slots:
   void fillAccountInfo(const QString&username, const QString&password);

signals:
   void toRegisterPage();
   void loginSuccess();
};
