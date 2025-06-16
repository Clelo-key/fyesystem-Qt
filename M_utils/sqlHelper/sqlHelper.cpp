#include "sqlHelper.h"

SqlHelper::SqlHelper() { initDatabase(); }
SqlHelper::~SqlHelper() {
  // 在析构函数中关闭数据库连接
  if (m_database.isOpen()) {
    m_database.close();
  }
}

SqlHelper& SqlHelper::GetInstance() {
  static SqlHelper sql;
  return sql;
}

QMutex SqlHelper::m_mutex;
bool SqlHelper::initDatabase() {
  QMutexLocker locker(&m_mutex);

  // 检查是否已添加过该连接
  if (QSqlDatabase::contains("myConnection")) {
    m_database = QSqlDatabase::database("myConnection");
    return m_database.isOpen();
  }

  // 创建新的数据库连接
  m_database = QSqlDatabase::addDatabase("QMYSQL", "myConnection");
  m_database.setHostName("localhost");
  m_database.setPort(3306);
  m_database.setDatabaseName("fye_system_qt");
  m_database.setUserName("root");
  m_database.setPassword("123456");

  // 打开数据库
  if (!m_database.open()) {
    qDebug() << "Database error:" << m_database.lastError().text();
    return false;
  }

  qDebug() << "Database connected successfully!";
  return true;
}

QSqlDatabase SqlHelper::getDatabase() {
  // 如果数据库未打开，则尝试重新打开
  if (!m_database.isOpen()) {
    initDatabase();
  }
  return m_database;
}

bool SqlHelper::isOpen() const { return m_database.isOpen(); }

QSqlError SqlHelper::lastError() const { return m_database.lastError(); }

// 常用语句查询
bool SqlHelper::checkFieldExists(const QString& fieldName,
                                 const QVariant& value) const {
  QSqlDatabase conn = GetInstance().getDatabase();

  QSqlQuery query(m_database);
  query.prepare(
      QString("SELECT 1 FROM sys_user WHERE %1 = ? LIMIT 1").arg(fieldName));
  query.addBindValue(value);

  if (!query.exec()) {
    qCritical() << "SQL查询执行失败:" << query.lastError().text();
    return false;
  }

  return query.next();  // 存在记录则返回true
}
