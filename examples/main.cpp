#include "mainwindow.h"
#include <stdio.h>

#include <QApplication>
#include <qstylefactory.h>


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setDesktopSettingsAware(false);
  a.setStyle(QStyleFactory::create("Fusion"));
  MainWindow w;
  w.setMinimumSize(900,600);
  w.show();
  return a.exec();
}
