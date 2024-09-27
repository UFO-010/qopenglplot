#ifndef AXIS_H
#define AXIS_H

#include <QOpenGLFunctions_3_3_Core>

#include <vector>

#include "rendertext.h"
#include "grid.h"


class GLAxis : protected QOpenGLFunctions_3_3_Core
{
public:
  GLAxis();
  ~GLAxis();

  void axisInit();

  void renderAxis();

  void setXRange(double minX, double maxX);
  void setYRange(double minY, double maxY);

  void setSize(double width, double height);

  void setPixelHeight(double height);
  void setPixelWidth(double width);

  double getPixelHeight();
  double getPixelWidth();


  void updateTickVectorY();
  void updateTickVectorX();

  Grid grid;
  RenderText rendertext;

private:
  std::vector<double> tickPosVectorX;
  std::vector<double> tickPosVectorY;
  std::vector<double> subTickPosVectorX;
  std::vector<double> subTickPosVectorY;

  std::vector<GLdouble> tickVectorY;
  std::vector<GLdouble> tickVectorX;

  double pixelWidth;
  double pixelHeight;

  double minX;
  double maxX;
  double minY;
  double maxY;

  GLuint VBO;
  GLuint VAO;
};


#endif  // AXIS_H
