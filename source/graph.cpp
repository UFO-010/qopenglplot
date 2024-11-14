#include "graph.h"


GLGraph::GLGraph()
{
  smooth_lines = false;
  size_buf = 0;
  offs_buf = 0;
  line_width = 1;
}


GLGraph::~GLGraph()
{
  glDeleteBuffers(1, &VBO);
}


void GLGraph::init()
{
  initializeOpenGLFunctions();

  glGenBuffers(1, &VBO);

}


/*!
 * \brief Graph::renderGraph
 *
 *
 */
void GLGraph::renderGraph()
{
  glLineWidth(line_width);
  if(smooth_lines)
    {
      glEnable(GL_BLEND);                                 // Enable color mix
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Mix colors using scale func for input and output color to smooth lines
      glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);            // Set fastest line smoothing
      glEnable(GL_LINE_SMOOTH);                           // Enable line smoothing
    }

  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, size_buf * sizeof(GLdouble),
               (Vertex.data() + offs_buf), GL_DYNAMIC_DRAW);
//  glBufferSubData(GL_ARRAY_BUFFER, 0, size_buf * sizeof(GLdouble), (Vertex.data() + offs_buf));

  glEnableClientState(GL_VERTEX_ARRAY);

  glColor4f(pen_color.red, pen_color.green, pen_color.blue, pen_color.alpha);

  glVertexPointer(2, GL_DOUBLE, 0, NULL);                // Enable vertex matrix
  glDrawArrays(GL_LINE_STRIP, 0, size_buf/2);
  glDisableClientState(GL_VERTEX_ARRAY);                // Disable vertex matrix

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  if(smooth_lines)
    {
      glDisable(GL_BLEND);
      glDisable(GL_LINE_SMOOTH);
    }
  glLineWidth(1);
}


/*!
 * \brief Graph::setData
 * \param keys
 * \param values
 *
 *
 */
void GLGraph::setData(std::vector<double> &keys, std::vector<double> &values) const
{
  if((keys.size() < 10) || (values.size() < 10) || (keys.size() != values.size()))
    {
      return;
    }

  size_t size = values.size();
  size_buf = (size * 2);
  offs_buf = 0;

  if(Vertex.size() != size * 2)
    {
      Vertex.resize(size * 2);
    }

  for (size_t i = 0; i < size * 2; i += 20)
    {
      Vertex[i] = keys [i/2 ];
      Vertex[i+1] = values[ i/2 ];
      Vertex[i+2] = keys[ (i + 1)/2 ];
      Vertex[i+3] = values[ (i + 1)/2 ];
      Vertex[i+4] = keys[ (i + 2)/2 ];
      Vertex[i+5] = values[ (i + 2)/2 ];
      Vertex[i+6] = keys[ (i + 3)/2 ];
      Vertex[i+7] = values[ (i + 3)/2 ];
      Vertex[i+8] = keys[ (i + 4)/2 ];
      Vertex[i+9] = values[ (i + 4)/2 ];
      Vertex[i+10] = keys[ (i + 5)/2 ];
      Vertex[i+11] = values[ (i + 5)/2 ];
      Vertex[i+12] = keys[ (i + 6)/2 ];
      Vertex[i+13] = values[ (i + 6)/2 ];
      Vertex[i+14] = keys[ (i + 7)/2 ];
      Vertex[i+15] = values[ (i + 7)/2 ];
      Vertex[i+16] = keys[ (i + 8)/2 ];
      Vertex[i+17] = values[ (i + 8)/2 ];
      Vertex[i+18] = keys[ (i + 9)/2 ];
      Vertex[i+19] = values[ (i + 9)/2 ];
    }
}


/*!
 * \brief Graph::setRange
 * \param min
 * \param max
 *
 *
 */
void GLGraph::setRange(double min, double max)
{
  if((max - min) * 2 >= Vertex.size())
    return;

  offs_buf = static_cast<int>(min * 2);
  size_buf = static_cast<int>((max - min)*2);
}


/*!
 * \brief Graph::setQColor
 * \param col
 *
 *
 */
void GLGraph::setQColor(QColor col) const
{
  pen_color.red = col.redF();
  pen_color.green = col.greenF();
  pen_color.blue = col.blueF();
}


/*!
 * \brief Graph::setColor
 * \param col
 *
 *
 */
void GLGraph::setColor(OGLColor col) const
{
  pen_color.red = col.redF();
  pen_color.green = col.greenF();
  pen_color.blue = col.blueF();
}


/*!
 * \brief Graph::Smoothing
 * \param state
 *
 * EN
 *
 * RU Режим сглаживания линий графика.
 * Накладывает дополнительные расходы
 * при отрисовке, что может сильно сказаться
 * на производительности при использовании
 * виртуализации видеокарты.
 */
void GLGraph::Smoothing(bool state) const
{
  smooth_lines = state;
}


void GLGraph::setLineWidth(int width) const
{
  line_width = width;
}
