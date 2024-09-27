
#include <QTimer>
#include <QGridLayout>

#include "mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
  QWidget *wgt = new QWidget();
  this->setCentralWidget(wgt);
  QGridLayout *grid = new QGridLayout();
  wgt->setLayout(grid);

  wgt0 = new wgtPlot();
  wgt1 = new wgtPlot();
  wgt2 = new wgtPlot();
  wgt3 = new wgtPlot();
  wgt4 = new wgtPlot();

  wgt0->setFocusPolicy(Qt::TabFocus);
  wgt1->setFocusPolicy(Qt::TabFocus);
  wgt2->setFocusPolicy(Qt::TabFocus);
  wgt3->setFocusPolicy(Qt::TabFocus);
  wgt4->setFocusPolicy(Qt::TabFocus);

  grid->addWidget(wgt0,0,0,1,1);
  grid->addWidget(wgt1,1,0,1,1);
  grid->addWidget(wgt2,2,0,1,1);
  grid->addWidget(wgt3,3,0,1,1);
  grid->addWidget(wgt4,4,0,1,1);

  int size = /*180000*//*40000*/16000;
  x1.resize(size);
  y1.resize(size);
  x2.resize(size);
  y2.resize(size);
  for(int i = 0; i < size; i++)
  {
    x1[i] = i;
    y1[i] = (sin(2 * 3.14 * i) * (1 << 11)) + (1 << 11) ;
    x2[i] = i;
    y2[i] = (cos(2 * 3.14 * i) * (1 << 20)) + (1 << 20);
  }

  QTimer *timer = new QTimer(this);
  timer->connect(timer, &QTimer::timeout, this, &MainWindow::replot);
  timer->start(125);
//  timer->singleShot(500,this, &MainWindow::replot);

  wgt0->graph()->setQColor(Qt::blue);
  wgt1->graph()->setQColor(Qt::red);
  wgt2->graph()->setColor(OGL::blue);
  wgt3->graph()->setColor(OGL::red);
  wgt4->graph()->setColor(OGL::blue);

  wgt0->graph()->setLineWidth(2);
  wgt1->graph()->setLineWidth(2);
  wgt2->graph()->setLineWidth(2);
  wgt3->graph()->setLineWidth(2);
  wgt4->graph()->setLineWidth(2);

//  wgt0->graph()->Smoothing(true);
//  wgt1->graph()->Smoothing(true);
//  wgt2->graph()->Smoothing(true);
//  wgt3->graph()->Smoothing(true);
//  wgt4->graph()->Smoothing(true);

  wgt0->graph()->setData(x1,y1);
  wgt1->graph()->setData(x2,y2);
  wgt2->graph()->setData(x1,y1);
  wgt3->graph()->setData(x2,y2);
  wgt4->graph()->setData(x1,y1);


  wgt0->glWidget()->setXRange(0, y1.size());
  wgt0->glWidget()->setYRange(*std::min_element(y1.begin(), y1.end()),
                          *std::max_element(y1.begin(), y1.end()));

  wgt1->glWidget()->setXRange(0, y2.size());
  wgt1->glWidget()->setYRange(*std::min_element(y2.begin(), y2.end()),
                          *std::max_element(y2.begin(), y2.end()));

  wgt2->glWidget()->setXRange(0, y1.size());
  wgt2->glWidget()->setYRange(*std::min_element(y1.begin(), y1.end()),
                          *std::max_element(y1.begin(), y1.end()));

  wgt3->glWidget()->setXRange(0, y2.size());
  wgt3->glWidget()->setYRange(*std::min_element(y2.begin(), y2.end()),
                          *std::max_element(y2.begin(), y2.end()));

  wgt4->glWidget()->setXRange(0, y1.size());
  wgt4->glWidget()->setYRange(*std::min_element(y1.begin(), y1.end()),
                          *std::max_element(y1.begin(), y1.end()));

}

MainWindow::~MainWindow()
{

}


void MainWindow::replot()
{
  wgt0->graph()->setData(x1,y1);
  wgt1->graph()->setData(x2,y2);
  wgt2->graph()->setData(x1,y1);
  wgt3->graph()->setData(x2,y2);
  wgt4->graph()->setData(x1,y1);

  wgt0->glWidget()->setXRange(0, y1.size());
  wgt0->glWidget()->setYRange(*std::min_element(y1.begin(), y1.end()),
                          *std::max_element(y1.begin(), y1.end()));

  wgt1->glWidget()->setXRange(0, y2.size());
  wgt1->glWidget()->setYRange(*std::min_element(y2.begin(), y2.end()),
                          *std::max_element(y2.begin(), y2.end()));

  wgt2->glWidget()->setXRange(0, y1.size());
  wgt2->glWidget()->setYRange(*std::min_element(y1.begin(), y1.end()),
                          *std::max_element(y1.begin(), y1.end()));

  wgt3->glWidget()->setXRange(0, y2.size());
  wgt3->glWidget()->setYRange(*std::min_element(y2.begin(), y2.end()),
                          *std::max_element(y2.begin(), y2.end()));

  wgt4->glWidget()->setXRange(0, y1.size());
  wgt4->glWidget()->setYRange(*std::min_element(y1.begin(), y1.end()),
                          *std::max_element(y1.begin(), y1.end()));

  wgt0->update();
  wgt1->update();
  wgt2->update();
  wgt3->update();
  wgt4->update();

}
