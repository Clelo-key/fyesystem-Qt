#ifndef HTTPREQUESTMANAGER_H
#define HTTPREQUESTMANAGER_H

#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QTimer>
#include <QUrl>

class HttpRequestManager : public QObject {
  Q_OBJECT
 public:
  explicit HttpRequestManager(QObject* parent = nullptr);
  ~HttpRequestManager();

  // 发起POST请求
  void post(const QString& url, const QMap<QString, QString>& headers,
            const QByteArray& data = QByteArray(), int timeoutMs = 10000);

 signals:
  // 请求完成信号（成功时）
  void requestCompleted(const QByteArray& responseData);
  // 请求错误信号
  void requestFailed(const QString& errorString, int statusCode = -1);

 private slots:
  void handleResponse();

 private:
  QNetworkAccessManager* m_networkManager;
};

#endif  // HTTPREQUESTMANAGER_H
