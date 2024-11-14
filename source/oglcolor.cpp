 #include "oglcolor.h"


/*!
 * \class OGLColor
 * \brief OGLColor class provides colors based on RGB float values
 *
 * Color is specified in terms of RGB (red, green, blue).
 *
 * OGLColor constructor creates the color based on RGB values.
 * Passed values should be float type because of OpenGL
 * specs. Alternatively, constructor can be called with
 * preimplemented color from OGL namespace.
*/

/*!
 * \brief OGLColor::OGLColor
 *
 * Construct OGLColor class with all components set to zero
 * Red = 0, Green = 0, Blue = 0, Alpha = 0
 */
OGLColor::OGLColor()
{
  rgba.redF = 0;
  rgba.greenF = 0;
  rgba.blueF = 0;
  rgba.alphaF = 0;
}


/*!
 * \brief OGLColor::OGLColor
 * \param col premplemented color from OGL namespace
 *
 * Construct OGLColor class using prepremplemented
 * color from OGL namespace. Color is set using
 * OGL::(color name).
 * Color name can be:
 * black
 * white
 * darkGray
 * gray
 * lightGray
 * red
 * green
 * blue
 * cyan
 * magenta
 * yellow
 * darkRed
 * darkGreen
 * darkBlue
 * darkCyan
 * darkMagenta
 * darkYellow
 * purple
 * brown.
 */
OGLColor::OGLColor(OGL::Colors col)
{
  static const float colors[][4] = {
    {0.0f, 0.0f, 0.0f, 1.0f},     // black
    {1.0f, 1.0f, 1.0f, 1.0f},     // white
    {0.1f, 0.1f, 0.1f, 1.0f},     // darkGray
    {0.5f, 0.5f, 0.1f, 1.0f},     // gray
    {0.7f, 0.7f, 0.7f, 1.0f},     // lightGray
    {1.0f, 0.0f, 0.0f, 1.0f},     // red
    {0.0f, 1.0f, 0.0f, 1.0f},     // green
    {0.0f, 0.0f, 1.0f, 1.0f},     // blue
    {0.5f, 1.0f, 1.0f, 1.0f},     // cyan
    {1.0f, 0.0f, 1.0f, 1.0f},     // magenta
    {1.0f, 1.0f, 0.0f, 1.0f},     // yellow
    {0.5f, 0.0f, 0.0f, 1.0f},     // darkRed
    {0.0f, 0.5f, 0.0f, 1.0f},     // darkGreen
    {0.0f, 0.0f, 0.5f, 1.0f},     // darkBlue
    {0.0f, 0.5f, 0.5f, 1.0f},     // darkCyan
    {0.5f, 0.0f, 0.5f, 1.0f},     // darkMagenta
    {0.5f, 0.5f, 0.0f, 1.0f},     // darkYellow
    {1.0f, 0.0f, 1.0f, 1.0f},     // purple
    {0.5f, 0.25f, 0.0f, 1.0f},    // brown
  };

  setRgbF(
      colors[col][0],
      colors[col][1],
      colors[col][2],
      colors[col][3]);
}


/*!
 * \brief OGLColor::OGLColor
 * \param col OGLColor class
 *
 *
 */
OGLColor::OGLColor(const OGLColor &col)
{
  rgba = col.rgba;
}


/*!
 * \brief OGLColor::~OGLColor
 *
 *
 */
OGLColor::~OGLColor()
{

}


/*!
 * \brief OGLColor::setRgbF Set color in float
 * \param redF
 * \param greenF
 * \param blueF
 * \param alphaF
 *
 *
 */
void OGLColor::setRgbF(float redF, float greenF, float blueF, float alphaF)
{
  if((redF > 1) || (greenF > 1)|| (blueF > 1) || (alphaF > 1))
    {
      return;
    }
  rgba.redF = redF;
  rgba.greenF = greenF;
  rgba.blueF = blueF;
  rgba.alphaF = alphaF;
}


/*!
 * \brief OGLColor::setRgb
 * \param red
 * \param green
 * \param blue
 * \param alpha
 *
 *
 */
void OGLColor::setRgb(int red, int green, int blue, float alpha)
{
  if((red > 255) || (green > 255)|| (blue > 255) || (alpha > 255))
    {
      return;
    }
  rgba.redF = red / 255;
  rgba.greenF = green / 255;
  rgba.blueF = blue / 255;
  rgba.alphaF = alpha / 255;
}


/*!
 * \brief OGLColor::redF
 * \return red component of color. float type
 *
 *
 */
float OGLColor::redF()
{
  return rgba.redF;
}


/*!
 * \brief OGLColor::greenF
 * \return green component of color. float type
 *
 *
 */
float OGLColor::greenF()
{
  return rgba.greenF;
}


/*!
 * \brief OGLColor::blueF
 * \return blue component of color. float type
 *
 *
 */
float OGLColor::blueF()
{
  return rgba.blueF;
}


/*!
 * \brief OGLColor::alphaF
 * \return alpha component of color, or color intensity. float type
 *
 *
 */
float OGLColor::alphaF()
{
  return rgba.alphaF;
}


/*!
 * \brief OGLColor::operator !=
 * \param col
 * \return bool type enum
 *
 *
 */
bool OGLColor::operator!=(const OGLColor &col) const
{
  return !operator==(col);
}


/*!
 * \brief OGLColor::operator ==
 * \param col
 * \return
 *
 *
 */
bool OGLColor::operator==(const OGLColor &col) const
{
return((rgba.redF == col.rgba.redF) &&
       (rgba.greenF == col.rgba.greenF) &&
       (rgba.blueF == col.rgba.blueF) &&
       (rgba.alphaF == col.rgba.alphaF));
}


/*!
 * \brief OGLColor::operator =
 * \param col
 * \return
 *
 *
 */
OGLColor &OGLColor::operator=(const OGLColor &col)
{
  rgba = col.rgba;
  return *this;
}


/*!
 * \brief OGLColor::operator =
 * \param col
 * \return
 *
 *
 */
OGLColor &OGLColor::operator=(OGL::Colors col)
{
  return operator=(OGLColor(col));
}
