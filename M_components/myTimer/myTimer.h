#pragma once

#include <QTimer>
#include <QWidget>
#include <functional>

class MyTimer : public QTimer {
  Q_OBJECT

 public:
  explicit MyTimer(QObject *parent);
  explicit MyTimer(QObject *parent, std::function<void()> callback = nullptr);
  ~MyTimer();

  // void start(int interval);
  //// void stopTimer() override;
  // void stop();

  std::function<void()> myCallback;  // 成员变量存储 callback
};
