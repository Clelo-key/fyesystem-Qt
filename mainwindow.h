#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <httpRequestManager/httpRequestManager.h>
#include <myMessageItem/MyMessageItem.h>

#include <QListWidget>
#include <QMainWindow>

#include "myMessageBox/MyMessageBox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  // 添加消息的公共接口
  void addMessage(const QString &text, E_WHOSAY who);
  void resizeEvent(QResizeEvent *event);
  void adjustMessageItems();
  void postAiAPi(const QString &question);
  QString extractContent(const QByteArray &json);

 protected:
  void closeEvent(QCloseEvent *event) override;

 private slots:
  void on_sendButton_clicked();
  void ensureMessageVisible();
 signals:
  void loadingMessage();

 private:
  bool sureQuit;
  Ui::MainWindow *ui;
  // 构建messages数组
  QJsonArray *messages;
  MyMessageBox *messagebox;
  // 创建HTTP请求管理器实例
  HttpRequestManager *httpManager;
  // 添加请求状态标志
  bool m_aiRequestPending = false;

  void handleAiError(const QString &error, int statusCode);
  void handleAiResponse(const QByteArray &responseData);
};

#endif  // MAINWINDOW_H
