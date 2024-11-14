#ifndef OGLCOLOR_H
#define OGLCOLOR_H


/*!
 * Defines preimplemeted colors for painting
 *
 * \see OGLColor(OGL::Colors col)
 */
namespace OGL {
  enum Colors
  {
    black,
    white,
    darkGray,
    gray,
    lightGray,
    red,
    green,
    blue,
    cyan,
    magenta,
    yellow,
    darkRed,
    darkGreen,
    darkBlue,
    darkCyan,
    darkMagenta,
    darkYellow,
    purple,
    brown
  };
}


class OGLColor
{
public:
  OGLColor();
  OGLColor(float r, float g, float b, float a = 1);
  OGLColor(OGL::Colors col);
  OGLColor(const OGLColor &col);

  OGLColor &operator=(const OGLColor &col);
  OGLColor &operator=(OGL::Colors col);
  bool operator==(const OGLColor &col) const;
  bool operator!=(const OGLColor &col) const;

  ~OGLColor();

  int red();
  int green();
  int blue();
  int alpha();

  float redF();
  float greenF();
  float blueF();
  float alphaF();

  void setRgbF(float redF, float greenF, float blueF, float alphaF);
  void setRgb(int red, int green, int blue, float alpha);

private:
  struct {float redF; float greenF; float blueF; float alphaF = 1;}rgba;
};

#endif    // OGLCOLOR_H
