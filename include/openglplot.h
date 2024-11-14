#ifndef OPENGLPLOT_H
#define OPENGLPLOT_H


#include <QWidget>
#include <QSharedPointer>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWindow>

#include <QMouseEvent>
#include <QWheelEvent>

#include <vector>

#include "graph.h"
#include "axis.h"


class OpenGLPlot : public QOpenGLWindow, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

public:
  explicit OpenGLPlot(/*QWidget*/);
  ~OpenGLPlot();

  void setYRange(double min, double max) const;
  void setXRange(double min, double max) const;

  void gridVisible(bool state);
  void axisVisible(bool state);

  const GLGraph *graph() const;

//  void addGraph();

protected:
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;

  void wheelEvent(QWheelEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

private:
  QSharedPointer<GLGraph> glGraph;
  QSharedPointer<GLAxis> glAxis;

  void updatePixels() const;

  void updateYRange(double min, double max);
  void updateXRange(double min, double max);

  float getGLversion();

  // Unused for now //
  GLuint FBO;
  GLuint RBO;

  bool showGrid;
  bool showAxis;
  //----------------//

  struct Range
  {
    double lower;
    double upper;
  };

  mutable struct {
    Range xRange;
    Range yRange;
  }sizeRange;

  mutable struct {
    Range xRange;
    Range yRange;
  }sizeRangeOld;

  int mouseMove;
  int mousePressPos;

  double wgtWidth;
  double wgtHeight;

  mutable Proj proj;

  bool initialized;
};


class wgtPlot : public QWidget
{
  Q_OBJECT

 public:
  explicit wgtPlot(QWidget *parent = nullptr);
  ~wgtPlot();

  const OpenGLPlot *glWidget() const;
  const GLGraph * graph() const;

public slots:
  void update() const;

private:
  QSharedPointer<OpenGLPlot> glplot;
};


inline const OpenGLPlot *wgtPlot::glWidget() const
{
  return glplot.get();
}


/*!
 * \brief wgtPlot::graph
 * \return
 *
 * EN
 *
 * RU
 */
inline const GLGraph *wgtPlot::graph() const
{
  return glplot->graph();
}


/*!
 * \brief wgtPlot::update
 *
 * EN
 *
 * RU Функция обновления виджета, которая
 * вызывает виртуальную функцию \see paintGL()
 */
inline void wgtPlot::update() const
{
  glplot->update();
}


/*!
 * \brief OpenGLPlot::graph
 * \return
 */
inline const GLGraph *OpenGLPlot::graph() const
{
  return glGraph.get();
}


#endif // OPENGLPLOT_H
