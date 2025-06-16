#include "myLabel.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent) {
}

MyLabel::MyLabel(QWidget *parent, QString&text) : QLabel(parent), text(text) {
   this->setMaximumWidth(100);
   this->setMaximumHeight(100);
   this->move(100, 300);
   this->setText(text);
   this->setStyleSheet("background:red");
   this->show();
}

MyLabel::~MyLabel() {
}

void MyLabel::myMouseLeave() {
   qDebug() << "My Mouse Has Leave";
}

void MyLabel::myMouseEnter() {
   qDebug() << "My Mouse Has Enter";
}

void MyLabel::updateLabelText(int&num) {
   num++;
   this->setText(QString::number(num));
}

void MyLabel::restartLabelText() {
   this->setText(this->text);
};

void MyLabel::mousePressEvent(QMouseEvent *ev) {
   emit PressSignals();
   // 窗口坐标
   // qDebug() << "My Mouse Has Press"
   //         << QString("x=%1,y=%2").arg(ev->position().x()).arg(ev->pos().y());

   //// 全局坐标
   // qDebug()
   //     << "Global XY"
   //     <<
   //     QString("x=%1,y=%2").arg(ev->globalPos().x()).arg(ev->globalPos().y());

   //// 鼠标按键
   // const char *bu = ev->button() == 1 ? "LeftButton" : "RightButton";
   // qDebug() << QString("My %1 has been Clicked").arg(bu);
};

void MyLabel::mouseMoveEvent(QMouseEvent *ev) {
   qDebug() << "My Mouse Has Move";
};

void MyLabel::mouseReleaseEvent(QMouseEvent *ev) {
   emit ReleaseSignals();
};
