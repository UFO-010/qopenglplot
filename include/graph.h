#ifndef GRAPH_H
#define GRAPH_H


#include <QOpenGLFunctions_3_3_Core>
#include <QColor>

#include <vector>

#include "oglcolor.h"


class GLGraph : protected QOpenGLFunctions_3_3_Core
{

public:
  explicit GLGraph();
  ~GLGraph();

  void renderGraph();
  void init();

  void setData(std::vector<double> &keys, std::vector<double> &values) const;

  void setColorf(float red, float green, float blue);
  void setColor(int red, int green, int blue);

  void setRange(double min, double max);

  void setLineWidth(int width) const;

  void setQColor(QColor col) const;
  void setColor(OGLColor col) const;

  void Smoothing(bool state) const;

private:

  mutable struct
  {
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;
    float alpha = 1.0;
  }pen_color;

  mutable std::vector<GLdouble> Vertex;

  GLuint VBO;

  mutable int size_buf;
  mutable int offs_buf;

  mutable bool smooth_lines;

  mutable int line_width;

};


#endif  // GRAPH_H
