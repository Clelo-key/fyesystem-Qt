#include "MyMenu.h"
#include <QCoreApplication>

MyMenu::MyMenu(QWidget *parent) : QMenuBar(parent) {
   QFile qssFile(":asset/style/myMenu.qss");
   if (qssFile.open(QFile::ReadOnly)) {
      this->setStyleSheet(qssFile.readAll());
   }

   qssFile.close();

   QMenu *file = new QMenu("File", this);

   QAction *newp = new QAction("new Poject", file);

   QIcon *icon = new QIcon(":asset/image/favicon.ico");
   newp->setIcon(*icon);
   QAction *newf = new QAction("new File", file);
   newf->setIcon(*icon);
   QAction *ofop = new QAction("Open File or Project", file);
   ofop->setIcon(*icon);
   file->addAction(newf);
   file->addAction(newp);
   file->addAction(ofop);

   QMenu *edit    = new QMenu("Edit", this);
   QMenu *view    = new QMenu("View", this);
   QMenu *build   = new QMenu("build", this);
   QMenu *debug   = new QMenu("Debug", this);
   QMenu *analyze = new QMenu("Analyze", this);
   QMenu *tools   = new QMenu("Tools", this);
   QMenu *windows = new QMenu("Windows", this);
   QMenu *help    = new QMenu("Help", this);

   this->addMenu(file);
   this->addMenu(edit);
   this->addMenu(view);
   this->addMenu(build);
   this->addMenu(debug);
   this->addMenu(analyze);
   this->addMenu(tools);
   this->addMenu(windows);
   this->addMenu(help);
}

MyMenu::~MyMenu() {
}
