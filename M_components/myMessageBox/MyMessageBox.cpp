#include "MyMessageBox.h"
// messagebox.cpp
#include <QGuiApplication>
#include <QMouseEvent>
#include <QScreen>

MyMessageBox::MyMessageBox(QWidget *parent) : QWidget(parent) {
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                 Qt::Dialog);
  setAttribute(Qt::WA_TranslucentBackground);
  // setAttribute(Qt::WA_DeleteOnClose);

  animation = new QPropertyAnimation(this, "windowOpacity", this);
  animation->setDuration(500);
  animation->setEasingCurve(QEasingCurve::InOutQuad);

  timer = new QTimer(this);
  connect(animation, &QPropertyAnimation::finished, this, &QWidget::close);

  connect(timer, &QTimer::timeout, this, &MyMessageBox::fadeOut);

  setupUI();
}
MyMessageBox::~MyMessageBox() { timer->stop(); }
void MyMessageBox::setupUI() {
  // 创建主布局
  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  mainLayout->setContentsMargins(10, 10, 10, 10);

  // 创建消息框框架
  frame = new QFrame(this);
  frame->setObjectName("message_frame");
  frame->setStyleSheet(R"(
        #message_frame {
            border-radius: 8px;
            padding: 15px;
        }
    )");

  // 消息内容布局
  QHBoxLayout *contentLayout = new QHBoxLayout(frame);
  contentLayout->setSpacing(5);

  // 图标标签
  iconLabel = new QLabel(this);
  iconLabel->setAlignment(Qt::AlignCenter);
  iconLabel->setFixedWidth(20);
  QFont font;
  font.setPointSize(20);
  iconLabel->setFont(font);
  contentLayout->addWidget(iconLabel);

  // 消息文本标签
  messageLabel = new QLabel(this);
  messageLabel->setWordWrap(true);
  contentLayout->addWidget(messageLabel, 1);

  // 关闭按钮
  closeButton = new QPushButton("×", this);
  closeButton->setFixedSize(20, 20);
  closeButton->setStyleSheet(R"(
        QPushButton {
            border: none;
            background: transparent;
            font-size: 14px;
            font-weight: bold;
            color:red;
        }
        QPushButton:hover {
            color: #555;
        }
    )");
  closeButton->hide();
  connect(closeButton, &QPushButton::clicked, this, [&]() {
    emit cancle();
    this->close();
  });
  contentLayout->addWidget(closeButton);

  // 确认按钮
  sureButton = new QPushButton("√", this);
  sureButton->setFixedSize(20, 20);
  sureButton->setStyleSheet(R"(
        QPushButton {
            border: none;
            background: transparent;
            font-size: 14px;
            font-weight: bold;
            color:green;
        }
        QPushButton:hover {
             color: #555;
        }
    )");
  sureButton->hide();
  connect(sureButton, &QPushButton::clicked, this, [&]() {
    emit makeSure();
    this->close();
  });
  contentLayout->addWidget(sureButton);

  mainLayout->addWidget(frame);
}

// 重置消息状态
void MyMessageBox::resetStatus() {
  messageLabel->clear();
  if (animation->state() == QAbstractAnimation::Running) {
    animation->stop();
  }
  timer->stop();
};

void MyMessageBox::showMessage(const QString &message, MessageType type,
                               int duration) {
  resetStatus();
  // 设置消息内容
  messageLabel->setText(message);
  // setFixedSize(300, 100);
  //  设置消息样式
  QString icon, backgroundColor, border, textColor;

  switch (type) {
    case INFO:
      icon = "💡";
      backgroundColor = "rgba(217, 237, 247, 0.95)";
      border = "1px solid #bce8f1";
      textColor = "#31708f";
      break;

    case SUCCESS:
      icon = "✅";
      backgroundColor = "rgba(223, 240, 216, 0.95)";
      border = "1px solid #d6e9c6";
      textColor = "#3c763d";
      break;

    case WARNING:
      icon = "⚠️";
      backgroundColor = "rgba(252, 248, 227, 0.95)";
      border = "1px solid #faebcc";
      textColor = "#8a6d3b";
      break;

    case ERROR:
      icon = "❌";
      backgroundColor = "rgba(242, 222, 222, 0.95)";
      border = "1px solid #ebccd1";
      textColor = "#a94442";
      break;
  }

  iconLabel->setText(icon);
  frame->setStyleSheet(QString(R"(
        #message_frame {
            background-color: %1;
            border: %2;
            border-radius: 8px;
            padding: 15px;
        }
        QLabel {
            color: %3;
            font-size: 14px;
        }
    )")
                           .arg(backgroundColor)
                           .arg(border)
                           .arg(textColor));

  // 设置不透明度
  setWindowOpacity(1.0);

  // duration 小于等于0 说明消息需要用户手动确认
  if (duration > 0) {
    // 启动计时器
    timer->start(duration);
  } else {
    setWindowModality(Qt::ApplicationModal);  // 应用级模态
    closeButton->show();
    sureButton->show();
  }
  // 调整大小并居中显示
  adjustSize();
  moveToCenter();
  // 显示消息框
  show();
}

void MyMessageBox::moveToCenter() {
  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();

  int x = (screenGeometry.width() - width()) / 2;
  int y = screenGeometry.height() / 10;  // 屏幕上方1/10位置

  move(x, y);
}

void MyMessageBox::fadeOut() {
  animation->setStartValue(1.0);
  animation->setEndValue(0.0);
  animation->start();
}

void MyMessageBox::mousePressEvent(QMouseEvent *event) {
  //// 点击关闭消息
  // if (event->button() == Qt::LeftButton) {
  //   close();
  // }
  QWidget::mousePressEvent(event);
}
