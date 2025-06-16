#include "HttpRequestManager.h"

#include <QDebug>
#include <QNetworkReply>
#include <QTimer>

HttpRequestManager::HttpRequestManager(QObject* parent)
    : QObject(parent), m_networkManager(new QNetworkAccessManager(this)) {}

HttpRequestManager::~HttpRequestManager() {
  // 清理所有未完成的请求
  for (auto reply : m_networkManager->findChildren<QNetworkReply*>()) {
    reply->abort();
    reply->deleteLater();
  }
}

void HttpRequestManager::post(const QString& url,
                              const QMap<QString, QString>& headers,
                              const QByteArray& data, int timeoutMs) {
  QNetworkRequest request;
  request.setUrl(QUrl(url));

  // 设置请求头
  for (auto it = headers.constBegin(); it != headers.constEnd(); ++it) {
    request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
  }

  // 发起POST请求
  QNetworkReply* reply = m_networkManager->post(request, data);

  // 设置超时定时器
  QTimer* timer = new QTimer(reply);
  timer->setSingleShot(true);
  timer->setInterval(timeoutMs);

  // 连接信号槽处理响应
  connect(reply, &QNetworkReply::finished, this,
          &HttpRequestManager::handleResponse);

  // 超时处理
  connect(timer, &QTimer::timeout, [=]() {
    if (reply->isRunning()) {
      reply->abort();
      emit requestFailed("Request timed out", 408);
      reply->deleteLater();
    }
  });

  // 启动超时定时器
  timer->start();
}

void HttpRequestManager::handleResponse() {
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
  if (!reply) return;

  // 错误处理（包括超时错误）
  if (reply->error() != QNetworkReply::NoError) {
    // 检查是否是超时错误
    QString errorMsg = reply->error() == QNetworkReply::OperationCanceledError
                           ? "Request timed out"
                           : reply->errorString();

    int statusCode =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    emit requestFailed(errorMsg, statusCode);
    reply->deleteLater();
    return;
  }

  // 读取响应数据
  QByteArray responseData = reply->readAll();
  emit requestCompleted(responseData);
  reply->deleteLater();
}
