#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QString>

#include <myTimer/myTimer.h>

class MyLabel : public QLabel {
  Q_OBJECT
 private:
  QString text;

 public:
  explicit MyLabel(QWidget *parent);
  explicit MyLabel(QWidget *parent, QString &text);
  ~MyLabel();
  void myMouseLeave();
  void myMouseEnter();
  void updateLabelText(int &num);
  void restartLabelText();

 protected:
  void mousePressEvent(QMouseEvent *ev) override;
  void mouseMoveEvent(QMouseEvent *ev) override;
  void mouseReleaseEvent(QMouseEvent *ev) override;

 signals:
  void PressSignals();
  void ReleaseSignals();
};
