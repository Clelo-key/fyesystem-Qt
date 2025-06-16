#include "mainwindow.h"

#include <QCloseEvent>
#include <QJsonArray>
#include <QListWidgetItem>
#include <QScrollBar>
#include <QTimer>
#include <QJsonObject>

#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      sureQuit(false),
      httpManager(new HttpRequestManager()),
      messages(new QJsonArray()) {
  ui->setupUi(this);
  // 初始化确认对话框
  messagebox = new MyMessageBox(this);

  // 设置窗口属性
  setWindowIcon(QIcon(":/asset/image/favicon.ico"));
  setWindowTitle("FyeSystem");

  // 添加示例消息
  // 延迟添加初始消息 - 使用单次定时器
  QTimer::singleShot(100, this, [this]() {
    addMessage("欢迎使用FyeSystem聊天功能", E_WHOSAY::eFriend);
  });
  // 连接信号和槽
  connect(messagebox, &MyMessageBox::cancle, this,
          [&]() { this->sureQuit = false; });

  connect(messagebox, &MyMessageBox::makeSure, this, [&]() {
    this->sureQuit = true;
    this->close();
  });
  // 连接HTTP管理器信号
  // 连接HttpRequestManager信号
  connect(httpManager, &HttpRequestManager::requestCompleted, this,
          &MainWindow::handleAiResponse);

  connect(httpManager, &HttpRequestManager::requestFailed, this,
          &MainWindow::handleAiError);
}

MainWindow::~MainWindow() { delete ui; }



void MainWindow::addMessage(const QString &text, E_WHOSAY who) {
  if (text.isEmpty()) return;

  // 创建消息项 - 使用 viewport() 作为父控件
  MyMessageItem *messageItem =
      new MyMessageItem(ui->messageList->viewport(), who);
  messageItem->setMsg(text);

  // 设置对齐方式
  if (who == E_WHOSAY::eMe) {
    messageItem->setAlignment(true);  // 右对齐
  } else {
    messageItem->setAlignment(false);  // 左对齐
  }

  // 创建列表项
  QListWidgetItem *listItem = new QListWidgetItem();

  // 设置尺寸提示
  QSize itemSize = messageItem->sizeHint();
  listItem->setSizeHint(itemSize);

  // 添加列表项
  ui->messageList->addItem(listItem);
  ui->messageList->setItemWidget(listItem, messageItem);

  // 滚动到底部并确保消息完全可见
  QTimer::singleShot(50, this, &MainWindow::ensureMessageVisible);
}

void MainWindow::ensureMessageVisible() {
  // 确保最后一条消息完全可见
  int lastRow = ui->messageList->count() - 1;
  if (lastRow >= 0) {
    ui->messageList->scrollToItem(ui->messageList->item(lastRow),
                                  QAbstractItemView::PositionAtBottom);

    // 额外滚动一些像素确保完全可见
    QScrollBar *vScrollBar = ui->messageList->verticalScrollBar();
    vScrollBar->setValue(vScrollBar->maximum());
  }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);

  // 调整所有消息项的尺寸
  for (int i = 0; i < ui->messageList->count(); ++i) {
    QListWidgetItem *item = ui->messageList->item(i);
    MyMessageItem *widget =
        qobject_cast<MyMessageItem *>(ui->messageList->itemWidget(item));
    if (widget) {
      // 通知消息项父容器大小已改变
      widget->setFixedWidth(ui->messageList->viewport()->width() - 20);
      item->setSizeHint(widget->sizeHint());
    }
  }

  // 确保消息可见
  ensureMessageVisible();
}

void MainWindow::on_sendButton_clicked() {
  QString text = ui->inputEdit->text().trimmed();
  if (!text.isEmpty()) {
    addMessage(text, E_WHOSAY::eMe);
    postAiAPi(text);
    ui->inputEdit->clear();
  }
}


void MainWindow::postAiAPi(const QString &question) {
  // 检查是否已有请求在处理
  if (m_aiRequestPending) {
    qWarning() << "已有消息正在处理中，忽略新请求";
    return;
  }

  m_aiRequestPending = true;

  // 显示等待消息（0 表示手动关闭）
  messagebox->showMessage("请稍等，消息正在处理中...", MyMessageBox::INFO, 0);

  // 设置请求URL
  QString url =
      "https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions";

  // 从环境变量获取API密钥
  QByteArray apiKey = qgetenv("AiKey");
  if (apiKey.isEmpty()) {
    qCritical() << "API密钥未设置!";
    messagebox->showMessage("API密钥未配置，无法请求AI服务",
                            MyMessageBox::ERROR);
    m_aiRequestPending = false;
    return;
  }

  // 设置请求头 (使用QMap替代QJsonObject)
  QMap<QString, QString> headers;
  headers["Authorization"] = "Bearer " + QString(apiKey).remove('\n');
  headers["Content-Type"] = "application/json";

  // 设置请求体
  QJsonObject requestBody;
  requestBody["model"] = "qwen-plus";

  // 构建消息数组
  QJsonArray messages;

  // 系统消息
  QJsonObject systemMessage;
  systemMessage["role"] = "system";
  systemMessage["content"] =
      "You are a helpful assistant. Managed a system called FyeSystem";
  messages.append(systemMessage);

  // 用户消息
  QJsonObject userMessage;
  userMessage["role"] = "user";
  userMessage["content"] = question;
  messages.append(userMessage);

  requestBody["messages"] = messages;

  // 添加可选参数
  requestBody["temperature"] = 0.7;
  requestBody["max_tokens"] = 1500;

  // 将JSON对象转换为字节数组
  QJsonDocument doc(requestBody);
  QByteArray jsonData = doc.toJson();

  // 发送POST请求 - 使用新的HttpRequestManager接口
  //httpManager->post(url, headers, jsonData, 30000);  // 设置30秒超时
  httpManager->post(url, headers, jsonData, 60000);  // 设置30秒超时
}


QString MainWindow::extractContent(const QByteArray &responseData) {
  // 将响应数据转换为JSON文档
  QJsonParseError parseError;
  QJsonDocument doc = QJsonDocument::fromJson(responseData, &parseError);

  // 检查JSON解析错误
  if (parseError.error != QJsonParseError::NoError) {
    qWarning() << "JSON解析错误:" << parseError.errorString();
    return "错误: 响应数据格式无效";
  }

  // 转换为JSON对象
  QJsonObject responseJson = doc.object();

  // 检查是否有错误
  if (responseJson.contains("error") && responseJson["error"].isObject()) {
    QJsonObject errorObj = responseJson["error"].toObject();
    if (errorObj.contains("message") && errorObj["message"].isString()) {
      return "错误: " + errorObj["message"].toString();
    }
    return "未知错误";
  }

  // 提取内容
  if (responseJson.contains("choices") && responseJson["choices"].isArray()) {
    QJsonArray choices = responseJson["choices"].toArray();
    if (!choices.isEmpty()) {
      QJsonObject firstChoice = choices.first().toObject();
      if (firstChoice.contains("message") &&
          firstChoice["message"].isObject()) {
        QJsonObject message = firstChoice["message"].toObject();
        if (message.contains("content") && message["content"].isString()) {
          return message["content"].toString();
        }
      }
    }
  }

  // 无法提取内容时返回格式化JSON
  return doc.toJson(QJsonDocument::Indented);
}

void MainWindow::handleAiError(const QString &error, int statusCode) {
  m_aiRequestPending = false;
  messagebox->hide();  // 关闭等待消息框

  QString errorMsg = QString("请求失败 (%1): %2").arg(statusCode).arg(error);
  messagebox->showMessage(errorMsg, MyMessageBox::ERROR);
  qWarning() << "AI请求错误:" << errorMsg;
}


void MainWindow::handleAiResponse(const QByteArray &responseData) {
  m_aiRequestPending = false;
  messagebox->hide();  // 关闭等待消息框

  QString content = extractContent(responseData);
  // 处理AI响应内容...
  addMessage(content, E_WHOSAY::eFriend);
  //ui->textEdit->append("AI: " + content);
}


void MainWindow::closeEvent(QCloseEvent *event) {
  // 关闭消息框
  event->ignore();
  if (!this->sureQuit) {
    messagebox->showMessage("确认退出吗?", MyMessageBox::WARNING, 0);
  } else {
    messagebox->close();
    QMainWindow::closeEvent(event);
  }
}
