#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QWidget>

class MyButton : public QPushButton
{
   Q_OBJECT
public:
   explicit MyButton(QWidget *parent = nullptr);
   explicit MyButton(const QString&title, QWidget *parent = nullptr);
signals:
   void ButtonSignals();
};


#endif // MYBUTTON_H
