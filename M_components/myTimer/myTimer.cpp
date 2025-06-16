#include "MyTimer.h"

MyTimer::MyTimer(QObject *parent) : QTimer(parent) {}

MyTimer::MyTimer(QObject *parent, std::function<void()> callback)
    : QTimer(parent), myCallback(callback) {
  connect(this, &QTimer::timeout, [this]() {
    if (this->myCallback) {
      this->myCallback();
    }
  });
}

MyTimer::~MyTimer() {}

// void MyTimer::start(int interval) {
//   // 记录定时器是否已启动
//   if (!isActive()) {
//     QTimer::start(interval);
//     // 可以在这里添加额外逻辑
//   }
// }
// void MyTimer::stop() {
//   if (isActive()) {
//     QTimer::stop();
//     // 可以在这里添加额外逻辑
//   }
// }
