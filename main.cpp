#include <loginWindow/myLoginWindow.h>

#include <QApplication>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // QTranslator translator;
  // const QStringList uiLanguages = QLocale::system().uiLanguages();

  // for (const QString &locale : uiLanguages) {
  //   const QString baseName = "FyeQt_" + QLocale(locale).name();
  //   if (translator.load(":/i18n/" + baseName)) {
  //     a.installTranslator(&translator);
  //     break;
  //   }
  // }

  // 创建在堆上，方便释放内存通过deletelater，且阻止用户直接退出。
  MainWindow *w = new MainWindow();
  MyLoginWindow *dia = new MyLoginWindow();

  QWidget::connect(dia, &MyLoginWindow::finished, &a,
                   &QApplication::quit);  // 用户取消登录，直接退出

  QWidget::connect(dia, &MyLoginWindow::loginComplate, w, [&]() {
    w->show();
    // 延迟添加初始消息 - 使用单次定时器
    QTimer::singleShot(100, w, [w]() {
      w->addMessage("欢迎使用FyeSystem聊天功能", E_WHOSAY::eFriend);
    });
    // 添加示例消息
    QString question = "你好，你是谁?";
    w->postAiAPi(question);
  });
  dia->show();

  // w->show();

  return (QApplication::exec());
}
