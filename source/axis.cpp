
#include "axis.h"
#include "tickspos.h"


#define tick_height 9
#define subtick_height 4
#define num_vec 2


GLAxis::GLAxis()
{
  minX = 0;
  maxX = 0;
  minY = 0;
  maxY = 0;

  pixelWidth = 0;
  pixelHeight = 0;
}


/*!
 * \brief Axis::~Axis
 */
GLAxis::~GLAxis()
{
  glDeleteBuffers(1, &VBO);
}


/*!
 * \brief Axis::axisInit
 */
void GLAxis::axisInit()
{
  initializeOpenGLFunctions();

  glGenBuffers(1, &VBO);

  rendertext.initTextRender();
  grid.gridInit();
}


/*!
 * \brief Axis::renderAxis
 */
void GLAxis::renderAxis()
{
  grid.drawGrid();

  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glColor4f(0.0, 0.0, 0.0, 1.0);

  glBufferData(GL_ARRAY_BUFFER, tickVectorY.size() * sizeof(GLdouble),
               tickVectorY.data() , GL_DYNAMIC_DRAW);
  glVertexPointer(2, GL_DOUBLE, 0, NULL);
  glDrawArrays(GL_LINES, 0, tickVectorY.size()/2);

  glBufferData(GL_ARRAY_BUFFER, tickVectorX.size() * sizeof(GLdouble),
               tickVectorX.data() , GL_DYNAMIC_DRAW);
  glVertexPointer(2, GL_DOUBLE, 0, NULL);
  glDrawArrays(GL_LINES, 0, tickVectorX.size()/2);

  glDisableClientState(GL_VERTEX_ARRAY);                // Disable vertex matrix

//  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  rendertext.renderText();
}


/*!
 * \brief Axis::setPixelHeight
 * \param height
 */
void GLAxis::setPixelHeight(double height)
{
  pixelHeight = height;
  rendertext.setPixelHeight(height);
  grid.setPixelHeight(height);
  updateTickVectorX();
}


/*!
 * \brief Axis::setPixelWidth
 * \param width
 */
void GLAxis::setPixelWidth(double width)
{
  pixelWidth = width;
  rendertext.setPixelWidth(width);
  grid.setPixelWidth(width);
  updateTickVectorY();
}


/*!
 * \brief Axis::getPixelHeight
 * \return
 */
double GLAxis::getPixelHeight()
{
  pixelHeight = rendertext.getPixelHeight();
  grid.setPixelHeight(pixelHeight);
  return pixelHeight;
}


/*!
 * \brief Axis::getPixelWidth
 * \return
 */
double GLAxis::getPixelWidth()
{
  pixelWidth = rendertext.getPixelWidth();
  grid.setPixelWidth(pixelWidth);
  return pixelWidth;
}


/*!
 * \brief Axis::setXRange
 * \param min
 * \param max
 */
void GLAxis::setXRange(double min, double max)
{
  minX = min;
  maxX = max;
  double tick_step_X = getTickStep(min, max, 5);

  createTickPosVector(tick_step_X, min, max, tickPosVectorX);
  createSubTickPosVector(tick_step_X, tickPosVectorX, min, max, subTickPosVectorX);

  rendertext.setTextX(tickPosVectorX);

  updateTickVectorX();

  grid.setXRange(min, max);
  grid.setLinesPosX(tickPosVectorX);
}


/*!
 * \brief Axis::setYRange
 * \param min
 * \param max
 */
void GLAxis::setYRange(double min, double max)
{
  minY = min;
  maxY = max;
  double tick_step_Y = getTickStep(min, max, 5);

  createTickPosVector(tick_step_Y, min, max, tickPosVectorY);
  createSubTickPosVector(tick_step_Y, tickPosVectorY, min, max, subTickPosVectorY);

//  rendertext.setText(tickPosVectorY, tickPosVectorX);
  rendertext.setTextY(tickPosVectorY);

  updateTickVectorY();
  grid.setYRange(min, max);
  grid.setLinesPosY(tickPosVectorY);
}


/*!
 * \brief Axis::updateTickVectorY
 */
void GLAxis::updateTickVectorY()
{
  if( tickPosVectorY.size() == 0 )
    return;

  int vec_add_offs = num_vec * 4;

  size_t vec_posY_size = tickPosVectorY.size();

  tickVectorY.clear();
  tickVectorY.resize( (subTickPosVectorY.size() + vec_posY_size) * 4 + vec_add_offs);

  tickVectorY[0] = minX;
  tickVectorY[1] = minY;
  tickVectorY[2] = minX;
  tickVectorY[3] = maxY;

  for(size_t i = 0; i < vec_posY_size; i++)
    {
      tickVectorY[(4 * i) + 4] = minX;
      tickVectorY[(4 * i) + 1 + 4] =  tickPosVectorY[i];
      tickVectorY[(4 * i) + 2 + 4] = minX + (pixelWidth * tick_height);
      tickVectorY[(4 * i) + 3 + 4] = tickPosVectorY[i];
    }

  int offs = (vec_posY_size * 4) + vec_add_offs;

  for(size_t i = 0; i < subTickPosVectorY.size(); i++)
    {
      tickVectorY[ (4 * i) + offs ] = minX;
      tickVectorY[ (4 * i) + 1 + offs ] =  subTickPosVectorY[i];
      tickVectorY[ (4 * i) + 2 + offs ] = minX + (pixelWidth * subtick_height);
      tickVectorY[ (4 * i) + 3 + offs ] = subTickPosVectorY[i];
    }
}


/*!
 * \brief Axis::updateTickVectorX
 */
void GLAxis::updateTickVectorX()
{
  if (tickPosVectorX.size() == 0)
    return;

  int vec_add_offs = num_vec * 4;

  size_t vec_posX_size = tickPosVectorX.size();

  tickVectorX.clear();
  tickVectorX.resize( (subTickPosVectorX.size() + vec_posX_size) * 4 + vec_add_offs);

  tickVectorX[0] = minX;
  tickVectorX[1] = minY;
  tickVectorX[2] = maxX;
  tickVectorX[3] = minY;

  for(size_t i = 0; i < vec_posX_size; i++)
    {
      tickVectorX[ (4 * i) + 4 ] = tickPosVectorX[i];
      tickVectorX[ (4 * i) + 1 + 4 ] = minY;
      tickVectorX[ (4 * i) + 2 + 4 ] = tickPosVectorX[i];
      tickVectorX[ (4 * i) + 3 + 4 ] = minY + (pixelHeight * tick_height);
    }

  int offs = (vec_posX_size * 4) + vec_add_offs;

  for(size_t i = 0; i < subTickPosVectorX.size(); i++)
    {
      tickVectorX[ (4 * i) + offs ] = subTickPosVectorX[i];
      tickVectorX[ (4 * i) + 1 + offs ] = minY;
      tickVectorX[ (4 * i) + 2 + offs ] = subTickPosVectorX[i];
      tickVectorX[ (4 * i) + 3 + offs ] = minY + (pixelHeight * subtick_height);
    }
}


