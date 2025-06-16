#include "myTest.h"

MyTest::MyTest() {}

MyTest::~MyTest() {}

void MyTest::myTimerTest(QWidget &w) {
  static QString text = "HelloWorld";
  MyLabel *label = new MyLabel(&w, text);
  // int num = 0;

  // MyTimer* timer =
  //     new MyTimer(&w, [&label, &num]() { label->updateLabelText(num); });

  // 防止label和num 丢失生命周期报错，使用static声明
  static int num = 0;

  MyTimer *timer = new MyTimer(&w, [label]() { label->updateLabelText(num); });

  // QObject::connect(label, &MyLabel::PressSignals, label,
  //                  [&]() { label->updateLabelText(num); });

  // QObject::connect(label, &MyLabel::PressSignals, timer,
  //                  [&timer]() { timer->start(1000); });

  QObject::connect(label, &MyLabel::PressSignals, timer,
                   [timer]() { timer->start(600); });

  // QObject::connect(label, &MyLabel::ReleaseSignals, label,
  //                  [&]() { label->restartLabelText(); });

  // 防止label和num 丢失生命周期报错，使用static声明
  QObject::connect(label, &MyLabel::ReleaseSignals, timer, [timer, label]() {
    timer->stop();
    label->restartLabelText();
  });
}
