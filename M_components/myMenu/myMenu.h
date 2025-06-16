// M_components/myMenu/myMenu.h
#ifndef MYMENU_H
#define MYMENU_H

#include <QMenuBar>
#include <QWidget>
#include <QFile>

class MyMenu : public QMenuBar {
   Q_OBJECT

public:
   explicit MyMenu(QWidget *parent);
   ~MyMenu();
};

#endif  // MYMENU_H
