#ifndef GRID_H
#define GRID_H

#include <QOpenGLFunctions_3_3_Core>

#include <vector>


class Grid : protected QOpenGLFunctions_3_3_Core
{
public:
  Grid();
  ~Grid();

  void gridInit();

  void drawGrid();

  void setPixelWidth(double width);
  void setPixelHeight(double height);

  void setYRange(double min, double max);
  void setXRange(double min, double max);

  void createLinesX();
  void createLinesY();

  void setLinesPosX(std::vector<double> &tickPos);
  void setLinesPosY(std::vector<double> &tickPos);

private:

  double minY;
  double maxY;
  double minX;
  double maxX;

  std::vector<double> linesVectorX;
  std::vector<double> linesVectorY;

  std::vector<double> linesPosX;
  std::vector<double> linesPosY;

  double lineHeightStep;
  double lineWidthStep;

  GLuint VBO;

};


#endif  // GRID_H
