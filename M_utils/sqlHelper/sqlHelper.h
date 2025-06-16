#pragma once
#include <QMutex>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtSql/qsqlquery.h>

class SqlHelper {
 public:
  static SqlHelper &GetInstance();
  // 获取数据库连接
  QSqlDatabase getDatabase();
  // 检查数据库是否打开
  bool isOpen() const;
  // 获取数据库错误
  QSqlError lastError() const;

  // 常用sql语句
  bool checkFieldExists(const QString &fieldName, const QVariant &value) const; // 查询某个字段是否存在


 private:
  SqlHelper();
  ~SqlHelper();
  // 禁止外部拷贝构造
  SqlHelper(const SqlHelper &sql) = delete;
  // 禁止外部赋值操作
  const SqlHelper &operator=(const SqlHelper &sql) = delete;

  // 初始化数据库连接
  bool initDatabase();

  QSqlDatabase m_database;
  static QMutex m_mutex;
};
