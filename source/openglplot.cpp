#include "openglplot.h"

#include <QVBoxLayout>

#ifndef __has_feature
// GCC does not have __has_feature...
#define __has_feature(feature) 0
#endif

#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS)
#ifdef __cplusplus
extern "C"
#endif
const char *__asan_default_options() {
  // Clang reports ODR Violation errors in mbedtls/library/certs.c.
  // NEED TO REPORT THIS ISSUE
  return "symbolize=1:detect_stack_use_after_return=1:external_symbolizer_path=/usr/bin/llvm-symbolizer";

}
const char *__lsan_default_options() {
  // Clang reports ODR Violation errors in mbedtls/library/certs.c.
  // NEED TO REPORT THIS ISSUE
  return "verbosity=1:log_threads=1:fast_unwind_on_malloc=0";
}
#endif


wgtPlot::wgtPlot(QWidget *parent) : QWidget(parent)
{
//  glplot = new OpenGLPlot();
//  glplot = std::make_shared<OpenGLPlot>();
  glplot = QSharedPointer<OpenGLPlot>(new OpenGLPlot);
  QWidget *wgt = QWidget::createWindowContainer(glplot.get());
  QVBoxLayout  *lay = new QVBoxLayout;
  lay->setContentsMargins(0, 0, 0, 0);
  lay->addWidget(wgt);
  this->setLayout(lay);
  wgt->setFocusPolicy(Qt::TabFocus);
  wgt->setParent(this);
}


wgtPlot::~wgtPlot()
{

}


OpenGLPlot::OpenGLPlot(/*QWidget *parent*/): /*QOpenGLWidget(parent)*/QOpenGLWindow()
{
//  glGraph = new GLGraph();
//  glAxis = new GLAxis();

  glGraph = QSharedPointer<GLGraph>(new GLGraph);
  glAxis = QSharedPointer<GLAxis>(new GLAxis);

  showGrid = false;
  showAxis = false;

  sizeRange.xRange.lower = 0;
  sizeRange.xRange.upper = 1;
  sizeRange.yRange.lower = 0;
  sizeRange.yRange.upper = 1;

  sizeRangeOld.xRange.lower = 0;
  sizeRangeOld.xRange.upper = 1;
  sizeRangeOld.yRange.lower = 0;
  sizeRangeOld.yRange.upper = 1;

  mouseMove = 0;
  mousePressPos = 0;

  wgtWidth = 1;
  wgtHeight = 1;
  initialized = false;
}


OpenGLPlot::~OpenGLPlot()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


/*!
 * \brief OpenGLPlot::initializeGL Initializing OpenGL functions.
 *
 * EN Initializing context function, gets called once before the first time
 * \fn resizeGL() or \fn paintGL() is called
 * When using Qt, it is virtual function which is
 * called during creating widget
 * All used vertex and array (if supported) buffer
 * objects should be created there, also
 * sets up the OpenGL resources and state
 *
 * RU Функция инициализации контекста, вызывается один раз
 * перед вызовом \fn resizeGL() или \fn paintGL()
 * При использование Qt является виртуальной функцией,
 * которая вызывается при создании окна. Все используемые
 * текстуры, шейдеры, буферы, массивы точек должны быть созданы
 * внутри этой функции
 */
void OpenGLPlot::initializeGL()
{
  initializeOpenGLFunctions();
  glGraph->init();
  glAxis->axisInit();

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Set background color
  initialized = true;

  setXRange(0.0, 5.0);
  setYRange(0.0, 5.0);

  float gl_ver = getGLversion();
  if(gl_ver < 3.3)
    {
      fprintf(stderr, "Required OpenGL version 3.3 \n");
      fprintf(stderr, "Supported version %.1f\n", gl_ver);
      exit(EXIT_FAILURE);
    }
}


/*!
 * \brief OpenGLPlot::resizeGL Main resize event.
 * \param width
 * \param height
 *
 * EN Gets called whenever the widget has been resized
 * and also when it is shown for the first time
 * because all newly created widgets should be
 * resized. When using Qt, widget get a resize event
 * automatically.
 * Sets up the OpenGL viewport and projection,
 * clear current matrix when called
 *
 * RU Функция вызывается каждый раз при изменении размера
 * виджета и при создании окна, потому что созданному
 * виджету необходимо обновить размер. При использовании
 * Qt является виртуальной функцией, вызывается
 * автоматически при каждом событии изменения размера
 * виджета. Устанавливает новые размеры контекста
 * для отрисовки. Помимо этого, обновляются матрицы
 * проекции \see updatePixels, т.к. значения матрицы проекции должны быть равны
 * целому количеству пикселей. Это необходимо для нормальной
 * отрисовки текста
 */
void OpenGLPlot::resizeGL(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();                   // Clear render matrix
  glViewport(0, 0, width, height);    // Change size of render window

  wgtWidth = static_cast<double>(width);
  wgtHeight = static_cast<double>(height);

  updatePixels();
  this->update();
}


/*!
 * \brief OpenGLPlot::paintGL Main paint event
 *
 * EN Renders the OpenGL scene. Gets called
 * whenever the widget needs to be updated.
 * When using Qt, this is a virtual function
 * that gets called everytime you call
 * update()
 *
 * RU Функция отрисовки сцены. Вызывается
 * каждый раз, когда необходимо обновить содержимое
 * экрана. При этом стирается всё содержимое экрана,
 * а все объекты должны отрисовываться заново. Помимо
 * этого, необходимо обновлять все uniform переменные
 * в используемых шейдерах и матрицу проекции. При
 * использовании Qt является виртуальной функцией,
 * которая исполняется каждый раз при вызове
 * \fn update()
 */
void OpenGLPlot::paintGL()
{
  makeCurrent();                      // Change render context
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT);       // Clear current color buffer

  glMatrixMode(GL_PROJECTION);        // Change to projection mode to enable multiplication between current and perspective matrix
  glLoadIdentity();                   // Clear current render matrix
  glOrtho(proj.left,
          proj.right,
          proj.bottom,
          proj.top, -1, 1);      // Create perspective matrix with pixel based coordinates
  glMatrixMode(GL_MODELVIEW);    // Change to object-view matrix

  glAxis->renderAxis();
  glGraph->renderGraph();
}


float OpenGLPlot::getGLversion()
{
  GLfloat maj_ver = 0;
  GLfloat min_ver = 0;
  glGetFloatv(GL_MAJOR_VERSION, &maj_ver);
  glGetFloatv(GL_MINOR_VERSION, &min_ver);
  float ver = maj_ver + (min_ver/10);
  return ver;
}


/*!
 * \brief OpenGLPlot::updatePixels
 *
 * EN
 *
 * RU
 */
void OpenGLPlot::updatePixels() const
{
  if(!initialized)
    return;

  double pixelWidth = (sizeRange.xRange.upper-sizeRange.xRange.lower)/static_cast<double>(wgtWidth);
  double pixelHeight = (sizeRange.yRange.upper-sizeRange.yRange.lower)/static_cast<double>(wgtHeight);

  proj.left = sizeRange.xRange.lower;
  proj.right = sizeRange.xRange.upper;
  proj.bottom = sizeRange.yRange.lower;
  proj.top = sizeRange.yRange.upper;

  glAxis->rendertext.setProjMatrix(proj);
  glAxis->setPixelHeight(pixelHeight);
  glAxis->setPixelWidth(pixelWidth);
  glAxis->rendertext.reserveSpace(wgtWidth, wgtHeight);

  proj = glAxis->rendertext.getProjMatrix();
}


/*!
 * \brief OpenGLPlot::setYRange
 * \param min
 * \param max
 *
 * EN
 *
 * RU Устанавливает диапазон графика по оси Y.
 * Для нормальной работы функция должна вызываться
 * только после создания контекста и инициализации
 * требуемых ресурсов, т.е. вызова \fn initializeGL(),
 */
void OpenGLPlot::setYRange(double min, double max) const
{
  if(!initialized)
    return;

  if ( (sizeRange.yRange.lower != min) || (sizeRange.yRange.upper != max) )
    {
      sizeRangeOld.yRange.lower = min;
      sizeRangeOld.yRange.upper = max;
      sizeRange.yRange.lower = min;
      sizeRange.yRange.upper = max;

      glAxis->setYRange(min, max);

      updatePixels();
    }
}


/*!
 * \brief OpenGLPlot::updateYRange
 * \param min
 * \param max
 *
 * EN
 *
 * RU Обновляет диапазон графика по оси Y для
 * функционирования увеличения и прокрутки графика.
 * Для нормальной работы функция должна вызываться
 * только после создания контекста и инициализации
 * требуемых ресурсов, т.е. вызова \fn initializeGL(),
 */
void OpenGLPlot::updateYRange(double min, double max)
{
  if(!initialized)
    return;

  sizeRange.yRange.lower = min;
  sizeRange.yRange.upper = max;

  glAxis->setYRange(min, max);

  updatePixels();
}


/*!
 * \brief OpenGLPlot::setXRange
 * \param min
 * \param max
 *
 * EN Set range of graph, axis and text
 * Should be called only after creating window
 * or OpenGL context with \fn initializeGL()
 *
 * RU Устанавливает диапазон графика по оси X.
 * Для нормальной работы функция должна вызываться
 * только после создания контекста и инициализации
 * требуемых ресурсов, т.е. вызова \fn initializeGL(),
 */
void OpenGLPlot::setXRange(double min, double max) const
{
  if(!initialized)
    return;

  if( (sizeRange.xRange.lower != min) || (sizeRange.xRange.upper != max) )
    {
      sizeRangeOld.xRange.lower = min;
      sizeRangeOld.xRange.upper = max;
      sizeRange.xRange.lower = min;
      sizeRange.xRange.upper = max;

      glGraph->setRange(min, max);
      glAxis->setXRange(min, max);

      updatePixels();
    }
}


/*!
 * \brief OpenGLPlot::updateXRange
 * \param min
 * \param max
 *
 * EN
 *
 * RU Обновляет диапазон графика по оси X.
 * Необходима для возможности приближения
 * и прокрутки графика. Для нормальной работы функция
 * должна вызываться только после создания контекста
 * и инициализации требуемых ресурсов, т.е. вызова
 * \fn initializeGL().
 */
void OpenGLPlot::updateXRange(double min, double max)
{
  if(!initialized)
    return;

  sizeRange.xRange.lower = min;
  sizeRange.xRange.upper = max;

  glGraph->setRange(min, max);
  glAxis->setXRange(min, max);

  updatePixels();
}


//---------------------------------------
// unused for now
void OpenGLPlot::gridVisible(bool state)
{
  showGrid = state;
}


void OpenGLPlot::axisVisible(bool state)
{
  showAxis = state;
}
//
//--------------------------------------


void OpenGLPlot::mousePressEvent(QMouseEvent *event)
{
  if(event->button() == Qt::LeftButton)
    {
      mousePressPos = event->pos().x();
    }
}


void OpenGLPlot::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton )
    {
      double moved_low = sizeRange.xRange.lower;
      double moved_upper = sizeRange.xRange.upper;
      mouseMove = (mousePressPos - event->pos().x()) * ((moved_upper - moved_low)/5000);

      if( (moved_low + mouseMove > 0) &&
         (moved_low + mouseMove < sizeRange.xRange.upper - 10) )
        {
          moved_low += mouseMove;
        }

      if( (moved_upper + mouseMove < sizeRangeOld.xRange.upper) &&
          moved_upper + mouseMove > moved_low + 10)
        {
          moved_upper += mouseMove;
        }

      updateXRange(moved_low, moved_upper);
      mousePressPos = event->pos().x();
      this->update();
    }

  event->accept();
}


void OpenGLPlot::wheelEvent(QWheelEvent *event)
{
  double moved_low = sizeRange.xRange.lower;
  double moved_upper = sizeRange.xRange.upper;
  int move =  event->angleDelta().y() * ((moved_upper - moved_low)/10000);

  if(moved_low + move >= moved_upper - 10 - move)
    {
      return;
    }

  if(moved_upper - move < sizeRangeOld.xRange.upper)
    {
      moved_upper -= move;
    }
  else
    {
      moved_upper = sizeRangeOld.xRange.upper;
    }

  if(moved_low + move > 0)
    {
      moved_low += move;
    }
  else
    {
      moved_low = sizeRangeOld.xRange.lower;
    }

  updateXRange(moved_low, moved_upper);
  event->accept();
  this->update();
}
