#ifndef RENDERTEXT_H
#define RENDERTEXT_H

#include <QOpenGLFunctions_3_3_Core>

#include <vector>
#include <unordered_map>


struct Proj
{
  float left;
  float right;
  float bottom;
  float top;
};

enum Arrange
{
  horizontal,
  vertical
};

class RenderText : protected QOpenGLFunctions_3_3_Core
{

  struct Character
  {
    GLuint sizex;
    GLuint sizey;
    GLint BearingX;
    GLint BearingY;
    GLint Advance;
    GLdouble texX;
    GLdouble texY;
  };

  struct TexAtlas
  {
    GLint rows;
    GLint cols;
    GLdouble rowFactor;
    GLdouble colFactor;
  }texAtlas;

  struct CHPrintInfo
  {
    uint Bearing;
    uint Advance;
    GLdouble texX;
    GLdouble texY;
  };


  struct Text
  {
    double num;
    std::vector<char> print;
    std::vector<CHPrintInfo>printInfo;
    std::vector<GLdouble> pos;
    double width;
    double height;
  };

public:
  explicit RenderText();
  ~RenderText();

  void initTextRender();
  void reserveSpace(int width, int height);

  void updateShaderMatrix();

  void updateTextPositionsX();
  void updateTextPositionsY();

  void setTextX(std::vector<double> &x);
  void setTextY(std::vector<double> &y);

  void renderText();

  inline void setPixelHeight(double height);
  inline void setPixelWidth(double width);
  void setProjMatrix(Proj &pr);

  inline double getPixelHeight();
  inline double getPixelWidth();
  Proj getProjMatrix();

private:
  double hintToPixel(double num, double pixelsize);

  void genTextures();

  int createShader(const char* vertex, const char* fragment);
  void getCharFromFloat(std::vector<char> *number, double input);

  void createCharacter(GLbyte ch);
  void createQCharacters(QString &q_str);

  Proj proj;

  std::unordered_map<char, Character> Characters;
  std::vector<Text> textBoxesX;
  std::vector<Text> textBoxesY;

  GLuint Texture;
  GLuint text_prog;
  GLuint text_VBO;
  GLuint text_VAO;

  double pixelWidth;
  double pixelHeight;

  int textMaxWidth;
  int textHeight;

  int characterWidth;
  int characterHeight;
  float proj_matrix[4][4];
};


inline void RenderText::setPixelHeight(double height)
{
  pixelHeight = height;
}


inline void RenderText::setPixelWidth(double width)
{
  pixelWidth = width;
}


inline double RenderText::getPixelHeight()
{
  return pixelHeight;
}


inline double RenderText::getPixelWidth()
{
  return pixelWidth;
}


#endif // RENDERTEXT_H
