// messagebox.h
#ifndef My_MESSAGEBOX_H
#define My_MESSAGEBOX_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class MyMessageBox : public QWidget {
  Q_OBJECT
 public:
  enum MessageType { INFO, SUCCESS, WARNING, ERROR };

  explicit MyMessageBox(QWidget *parent = nullptr);
  ~MyMessageBox();
  void showMessage(const QString &message, MessageType type = INFO,
                   int duration = 1000);

 protected:
  void mousePressEvent(QMouseEvent *event) override;

 private slots:
  void fadeOut();

 private:
  void setupUI();
  void moveToCenter();
  void resetStatus();

  QLabel *iconLabel;
  QLabel *messageLabel;
  QPushButton *closeButton;
  QPushButton *sureButton;
  QFrame *frame;
  QPropertyAnimation *animation;
  QTimer *timer;
 signals:
  void cancle();
  void makeSure();
};

#endif  // MESSAGEBOX_H
