
#include "grid.h"
#include <iostream>
#include <math.h>


Grid::Grid()
{
  minY = 0;
  maxY = 0;
  minX = 0;
  maxX = 0;

  lineHeightStep = 1;
  lineWidthStep = 1;
}


Grid::~Grid()
{
  glDeleteBuffers(1, &VBO);
}


void Grid::gridInit()
{
  initializeOpenGLFunctions();

  glGenBuffers(1, &VBO);
}


void Grid::drawGrid()
{
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glColor4f(0.5, 0.5, 0.1, 0.5);

  glBufferData(GL_ARRAY_BUFFER, linesVectorX.size() * sizeof(GLdouble),
               linesVectorX.data(), GL_DYNAMIC_DRAW);
  glVertexPointer(2, GL_DOUBLE, 0, NULL);
  glDrawArrays(GL_LINES, 0, linesVectorX.size()/2);

  glBufferData(GL_ARRAY_BUFFER, linesVectorY.size() * sizeof(GLdouble),
               linesVectorY.data(), GL_DYNAMIC_DRAW);
  glVertexPointer(2, GL_DOUBLE, 0, NULL);
  glDrawArrays(GL_LINES, 0, linesVectorY.size()/2);

  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Grid::setXRange(double min, double max)
{
  minX = min;
  maxX = max;
}


void Grid::setYRange(double min, double max)
{
  minY = min;
  maxY = max;
}


void Grid::setLinesPosX(std::vector<double> &tickPos)
{
  linesPosX.resize(tickPos.size());
  for(size_t i = 0; i < tickPos.size(); i++)
    {
      linesPosX[i] = tickPos[i];
    }
  createLinesX();
}


void Grid::setLinesPosY(std::vector<double> &tickPos)
{
  linesPosY.resize(tickPos.size());
  for(size_t i = 0; i < tickPos.size(); i++)
    {
      linesPosY[i] = tickPos[i];
    }
  createLinesY();
}


void Grid::createLinesY()
{
  int count = static_cast<int>((maxX - minX)/lineWidthStep);

  if(count < 1)
    return;

  int mar = count % 4;
  if(mar > 0)
    {
      count += (4 - mar);
    }
  linesVectorY.resize(count * linesPosY.size());

  for(size_t line = 0; line < linesPosY.size(); line++)
    {
      for(uint pos = count * line; pos < (count * (line + 1)) - 1; pos += 2)
        {
          linesVectorY[pos] = minX + ((pos - (count * line))  * lineWidthStep);
          linesVectorY[pos + 1] =  linesPosY[line];
        }
    }
}


void Grid::createLinesX()
{
  int count = static_cast<int>((maxY - minY)/lineHeightStep);

  if(count < 1)
    return;

  int mar = count % 4;
  if(mar > 0)
    {
      count += (4 - mar);
    }
  linesVectorX.resize(count * linesPosX.size());

  for(size_t line = 0; line < linesPosX.size(); line++)
    {
      for(uint pos = count * line; pos < (count * (line + 1)) - 1; pos += 2)
        {
          linesVectorX[pos] = linesPosX[line];
          linesVectorX[pos + 1] = minY + ((pos - (count * line))  * lineHeightStep);
        }
    }
}


void Grid::setPixelHeight(double height)
{
  lineHeightStep = height * 2;
  createLinesX();
}


void Grid::setPixelWidth(double width)
{
  lineWidthStep = width * 2;
  createLinesY();
}

