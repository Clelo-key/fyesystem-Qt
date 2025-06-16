#include "mybutton.h"

#include <QPushButton>

MyButton::MyButton(QWidget *parent) : QPushButton(parent) {}

MyButton::MyButton(const QString &title, QWidget *parent)
    : QPushButton(title, parent) {}
