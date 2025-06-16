#ifndef MYMESSAGEITEM_H
#define MYMESSAGEITEM_H

#include <QLabel>
#include <QTextBrowser>  // 改为 QTextBrowser 支持内部滚动
#include <QWidget>

enum E_WHOSAY { eFriend, eMe };

class MyMessageItem : public QWidget {
  Q_OBJECT
 public:
  explicit MyMessageItem(QWidget *parent = nullptr, E_WHOSAY eWhoSay = eFriend);
  ~MyMessageItem();

  void setMsg(const QString &strMsg);
  QString getMsg() const;

  void setAlignment(bool isRightAligned);
  bool isRightAligned() const { return m_isRightAligned; }

  QSize sizeHint() const override;

 protected:
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

 private:
  QSize calculateBubbleSize() const;

 private:
  QTextBrowser *m_pTextBrowser;  // 改为 QTextBrowser
  QString m_strMsg;
  E_WHOSAY m_eWhoSay;
  bool m_isRightAligned = false;
  int m_maxWidth = 500;
};

#endif  // MYMESSAGEITEM_H
