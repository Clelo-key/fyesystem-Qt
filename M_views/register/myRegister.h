#ifndef MYREGISTER_H
#define MYREGISTER_H
#include <QWidget>

#include "ui_myRegister.h"
#include <myMessageBox/MyMessageBox.h>


class MyRegister : public QWidget {
   Q_OBJECT

public:
   MyRegister(QWidget *parent = nullptr);
   ~MyRegister();

private:
   bool validateDatabase(const QString& username, const QString& phone,
                         const QString& password, const QString& nickname);
   bool validateCredentials(std::optional <QString>& account,
                            std::optional <QString>& phone,
                            std::optional <QString>& password,
                            std::optional <QString>& password2,
                            std::optional <QString>& nickname);

private slots:
   void on_registerButton_clicked();

signals:
   void toLoginPage();
   void registerSuccess(const QString& username, const QString& password);

private:
   Ui::MyRegister *ui;
   MyMessageBox *messageBox;
};
#endif // MYREGISTER_H
