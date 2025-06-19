#include "MyMessageItem.h"

#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QScreen>
#include <QScrollBar>
#include <QStyle>
#include <QStyleOption>

MyMessageItem::MyMessageItem(QWidget *parent, E_WHOSAY eWhoSay)
    : QWidget(parent), m_strMsg(""), m_eWhoSay(eWhoSay) {
  // 创建文本浏览器（支持滚动）
  m_pTextBrowser = new QTextBrowser(this);
  m_pTextBrowser->setOpenLinks(false);
  m_pTextBrowser->setReadOnly(true);
  m_pTextBrowser->setFrameShape(QFrame::NoFrame);
  m_pTextBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_pTextBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_pTextBrowser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_pTextBrowser->setStyleSheet("background: transparent; border: none;");
  m_pTextBrowser->setAttribute(Qt::WA_TranslucentBackground);

  // 设置字体
  QFont font = qApp->font();
  font.setPixelSize(14);
  m_pTextBrowser->setFont(font);

  // 设置布局
  QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
  horizontalLayout->addWidget(m_pTextBrowser);

  // 设置边距
  horizontalLayout->setContentsMargins(10, 5, 10, 5);

  // 设置初始最小尺寸
  setMinimumSize(50, 30);

  // 获取父容器的最大宽度
  if (parent) {
    m_maxWidth = parent->width();
  }
}

MyMessageItem::~MyMessageItem() {}

QSize MyMessageItem::sizeHint() const {
  QSize scal = calculateBubbleSize();
  return QSize(scal.width()+10, scal.height() + 50);
}

QSize MyMessageItem::calculateBubbleSize() const {
  if (m_strMsg.isEmpty()) {
    return QSize(100, 40);
  }

  // 计算文本所需尺寸
  QFontMetrics fm(m_pTextBrowser->font());

  // 最大宽度限制
  int maxWidth = qMax(100, m_maxWidth - 40);

  // 计算高度 - 使用文本浏览器的文档高度
  int height = m_pTextBrowser->document()->size().height() + 20;

  // 限制最大高度为屏幕高度的50%
  QScreen *screen = QApplication::primaryScreen();
  int maxHeight = screen->geometry().height() * 0.5;
  height = qMin(height, maxHeight);

  // 应用最小尺寸限制
  int width = qMin(maxWidth, fm.horizontalAdvance(m_strMsg) + 40);
  width = qMax(width, 100);
  height = qMax(height, 40);

  return QSize(width, height);
}

void MyMessageItem::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // 根据消息方向设置位置
  QRectF bubbleRect;
  QPainterPath trianglePath;

  // 设置内边距
  const int triangleWidth = 10;
  const int borderRadius = 8;

  if (m_isRightAligned) {
    // 右侧消息
    bubbleRect = QRectF(0, 0, width() - triangleWidth, height());
    trianglePath.moveTo(width() - triangleWidth, height() / 2 - 7);
    trianglePath.lineTo(width(), height() / 2);
    trianglePath.lineTo(width() - triangleWidth, height() / 2 + 7);
  } else {
    // 左侧消息
    bubbleRect = QRectF(triangleWidth, 0, width() - triangleWidth, height());
    trianglePath.moveTo(triangleWidth, height() / 2 - 7);
    trianglePath.lineTo(0, height() / 2);
    trianglePath.lineTo(triangleWidth, height() / 2 + 7);
  }

  // 设置气泡颜色
  QColor bubbleColor = m_isRightAligned ? QColor(98, 188, 90) :  // 绿色气泡
                           QColor(240, 240, 240);                // 灰色气泡

  // 绘制气泡
  painter.setPen(Qt::NoPen);
  painter.setBrush(bubbleColor);
  painter.drawRoundedRect(bubbleRect, borderRadius, borderRadius);
  painter.drawPath(trianglePath);
}

void MyMessageItem::setMsg(const QString &strMsg) {
  m_strMsg = strMsg;

  // 设置HTML内容，保留换行和空格
  QString htmlContent = strMsg.toHtmlEscaped();
  htmlContent.replace("\n", "<br>");
  htmlContent.replace(" ", "&nbsp;");

  m_pTextBrowser->setHtml(htmlContent);

  // 调整文本浏览器尺寸
  m_pTextBrowser->document()->adjustSize();

  // 更新尺寸
  QSize newSize = this->calculateBubbleSize();
  this->setFixedSize(newSize);
  this->updateGeometry();
}

void MyMessageItem::setAlignment(bool isRightAligned) {
  m_isRightAligned = isRightAligned;

  // 更新布局边距
  if (auto *layout = qobject_cast<QHBoxLayout *>(this->layout())) {
    if (m_isRightAligned) {
      layout->setContentsMargins(15, 5, 5, 5);
    } else {
      layout->setContentsMargins(5, 5, 15, 5);
    }
  }

  // 重新绘制
  update();
}

void MyMessageItem::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);

  // 当控件大小改变时，更新最大宽度
  if (parentWidget()) {
    m_maxWidth = parentWidget()->width();
  }

  // 调整文本浏览器的大小以适应新的宽度
  if (m_pTextBrowser) {
    // 获取当前布局的左右边距
    int leftMargin = layout()->contentsMargins().left();
    int rightMargin = layout()->contentsMargins().right();
    int newWidth = width() - leftMargin - rightMargin;

    // 设置文本浏览器的宽度
    m_pTextBrowser->setFixedWidth(newWidth);

    // 更新文档尺寸
    QTextDocument *doc = m_pTextBrowser->document();
    doc->setTextWidth(newWidth);

    // 重新计算控件高度
    int docHeight = doc->size().height();
    int topMargin = layout()->contentsMargins().top();
    int bottomMargin = layout()->contentsMargins().bottom();
    int newHeight = docHeight + topMargin + bottomMargin;

    // 限制最大高度
    QScreen *screen = QApplication::primaryScreen();
    int maxHeight = screen->geometry().height() * 0.5;
    newHeight = qMin(newHeight, maxHeight);

    // 应用新高度
    setFixedHeight(newHeight);

    // 通知父控件尺寸已变化
    updateGeometry();
  }
}
