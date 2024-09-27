#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>

#include <vector>

#include "openglplot.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  wgtPlot *wgt0;
  wgtPlot *wgt1;
  wgtPlot *wgt2;
  wgtPlot *wgt3;
  wgtPlot *wgt4;

  std::vector<double> x1;
  std::vector<double> y1;

  std::vector<double> x2;
  std::vector<double> y2;

public slots:
  void replot();

};
#endif // MAINWINDOW_H
