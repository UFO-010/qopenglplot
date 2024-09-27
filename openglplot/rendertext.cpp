#include <math.h>

#include <QPainter>

#include "rendertext.h"


#define MAX_CH 7


const char *vertexShaderText =
    "#version 330 core\n"
    "layout (location = 0) in vec4 vertex;\n"
    "uniform mat4 ModelViewProjectionMatrix;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    " {\n"
    "   gl_Position = ModelViewProjectionMatrix * vec4(vertex.xy, 0.0, 1.0);\n"
    "   TexCoord = vertex.zw;\n"
    " }\n";


const char *fragmentShaderText =
    "#version 330 core\n"
    "in vec2 TexCoord;\n"
    "out vec4 Color;\n"
    "uniform sampler2D text;\n"
    "vec3 textColor = vec3(0.0, 0.0, 0.0);\n"
    "void main()\n"
    " {\n"
    "   vec4 sampled = vec4(0.0, 0.0, 0.0, texture(text, TexCoord).r);\n"
    "   Color = vec4(textColor, 1.0) * sampled;\n"
    " }\n";


RenderText::RenderText()
{
  textHeight = 0;
  textMaxWidth = 0;
  text_prog = 0;

}


RenderText::~RenderText()
{
  glDeleteVertexArrays(1, &text_VAO);
  glDeleteBuffers(1, &text_VBO);
  glDeleteProgram(text_prog);
}


/*!
 * \brief RenderText::initTextRender
 *
 * EN Initialize OpenGL functions, create shader program
 * and generate text textures. Should be called in
 * \fn initializeGL() once.
 */
void RenderText::initTextRender()
{
  initializeOpenGLFunctions();

  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  text_prog = createShader(vertexShaderText, fragmentShaderText);
  glUseProgram(text_prog);

  genTextures();

  glGenBuffers(1, &text_VBO);
  glGenVertexArrays(1, &text_VAO);

  glBindVertexArray(text_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, text_VBO);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_DOUBLE, GL_FALSE, 4 * sizeof(GLdouble), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}


/*!
 * \brief RenderText::renderText
 */
void RenderText::renderText()
{
  glEnable(GL_BLEND);

  glUseProgram(text_prog);

  glUniformMatrix4fv(glGetUniformLocation(text_prog, "ModelViewProjectionMatrix"),
                     1, GL_FALSE, &proj_matrix[0][0]);

  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(text_VAO);
  glEnableClientState(GL_VERTEX_ARRAY);
  glBindTexture(GL_TEXTURE_2D, Texture);
  glBindBuffer(GL_ARRAY_BUFFER, text_VBO);

  for(size_t i = 0; i < textBoxesX.size(); i++)
    {
      glBufferData(GL_ARRAY_BUFFER, textBoxesX[i].pos.size() * sizeof(GLdouble),
                   textBoxesX[i].pos.data(), GL_DYNAMIC_DRAW);

      glDrawArrays(GL_TRIANGLES, 0, textBoxesX[i].pos.size()/4);
    }

  for(size_t i = 0; i < textBoxesY.size(); i++)
    {
      glBufferData(GL_ARRAY_BUFFER, textBoxesY[i].pos.size() * sizeof(GLdouble),
                   textBoxesY[i].pos.data(), GL_DYNAMIC_DRAW);

      glDrawArrays(GL_TRIANGLES, 0, textBoxesY[i].pos.size()/4);
    }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
  glDisableClientState(GL_VERTEX_ARRAY);

  glDisable(GL_BLEND);
}


/*!
 * \brief RenderText::setProjMatrix
 * \param pr projection matrix values
 *
 * Set projection matrix of class.
 * After that, \fn reserveAxisSpace()
 * should be called to reserve enough space in parent
 * widget, so we don't bother parent widget values that
 * should be printed.
 */
void RenderText::setProjMatrix(Proj &pr)
{
  proj.left = pr.left;
  proj.bottom = pr.bottom;
  proj.right = pr.right;
  proj.top = pr.top;
}


/*!
 * \brief RenderText::getProjMatrix
 * \return
 */
Proj RenderText::getProjMatrix()
{
  return proj;
}


/*!
 * \brief RenderText::updateShaderMatrix
 */
void RenderText::updateShaderMatrix()
{
  // Create projection matrix to load to shaders
  proj_matrix[0][0] = 2/(proj.right - proj.left);
  proj_matrix[0][1] = 0;
  proj_matrix[0][2] = 0;
  proj_matrix[0][3] = 0;

  proj_matrix[1][0] = 0;
  proj_matrix[1][1] = 2/(proj.top - proj.bottom);
  proj_matrix[1][2] = 0;
  proj_matrix[1][3] = 0;

  proj_matrix[2][0] = 0;
  proj_matrix[2][1] = 0;
  proj_matrix[2][2] = -2/(-1-1);
  proj_matrix[2][3] = 0;

  proj_matrix[3][0] = -(proj.right + proj.left)/(proj.right - proj.left);
  proj_matrix[3][1] = -(proj.top + proj.bottom)/(proj.top - proj.bottom);
  proj_matrix[3][2] = -(-1+1)/(-1-1);
  proj_matrix[3][3] = 1;
}


/*!
 * \brief RenderText::hintToPixel
 * \param num number to hint
 * \param pixelsize size of pixel that we need to hint \param num to.
 * \return hinted \param num
 *
 * We need to make sure that some varriables are put exactly
 * on edges of pixels. It should be done due to subpixel
 * interpolation of text.
 */
double RenderText::hintToPixel(double num, double pixelsize)
{
  double md;
  double frac = modf((num / pixelsize), &md);   // hint num to be in integer size in pixels
  if (1.0 - frac < 0.01)                        // handle almost integer size in pixels
    ++md;
  if (1.0 - frac > 1.0)                         // handle if value is negative
    --md;
  return (md * pixelsize);
}


/*!
 * \brief RenderText::reserveAxisSpace
 * \param width
 * \param height
 */
void RenderText::reserveSpace(int width, int height)
{
  // reserving enough space for text
  int pix_x_offs = textMaxWidth + characterWidth;
  int pix_y_offs = textHeight + 2;       // text height + offset

// how much pixel size will change after reserving space for text
  double del_w = pix_x_offs * pixelWidth / static_cast<double>(width);
  double del_h = pix_y_offs * pixelHeight / static_cast<double>(height);

// reserve enough space for text texture
  proj.left -= (pixelWidth + del_w) * pix_x_offs;
  proj.bottom -= (pixelHeight + del_h) * pix_y_offs;
  proj.right += pixelWidth * 10;
  proj.top += pixelHeight * (textHeight / 2);

// recalculate pixel sizes
  pixelWidth = (proj.right - proj.left) / static_cast<double>(width);
  pixelHeight = (proj.top - proj.bottom) / static_cast<double>(height);

// due to sublixel text interpolation, we need to make projection matrix
// to be equal to integer number in pixels
  proj.left = hintToPixel(proj.left, pixelWidth);
  proj.right = hintToPixel(proj.right, pixelWidth);
  proj.bottom = hintToPixel(proj.bottom, pixelHeight);
  proj.top = hintToPixel(proj.top, pixelHeight);

  updateShaderMatrix();
  updateTextPositionsX();
  updateTextPositionsY();
}


/*!
 * \brief Plot::createShader
 * \param vertexShader
 * \param fragmentShader
 * \return compiled shader program
 *
 * EN Compile passed OpenGL shaders to OpenGL program.
 * Use compiled program to update uniforms and load needed
 * vertex array objects (if OpenGl version supports it) with
 * value to pass to shaders. To do so, you need to use
 * \fn glUseProgram with returned variable and \fn glBindVertexArray
 * to change buffer without the need to change values, or \fn glUniform
 * with the name of uniform to update it's values
 *
 * RU Компилирует OpenGl шейдеры и создаёт программу OpenGL.
 * Чтобы обновить uniform переменные в шейдерах или загрузить
 * массив вершин в шейдеры (если версия OpenGL поддерживает их),
 * необходимо обращаться к программе используя \fn glUseProgram,
 * затем вызвать \fn glUniform для обновления переменных или
 * \fn glBindVertexArray для загрузки массива вершин в шейдеры.
 * \fn glUniform и \fn glBindVertexArray необходимо вызывать
 * каждый цикл отрисовки
 */
int RenderText::createShader(const char* vertexShader, const char* fragmentShader)
{
  GLint state = 0;
  GLint log_len = 0;

  GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexShader, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &state);
  if(state == GL_FALSE)
    {
      glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &log_len);
      GLchar *infolog = new GLchar[log_len];
      glGetShaderInfoLog(vertex, log_len, NULL, infolog);
      fprintf(stderr, "Vertex shader failed, %s\n", infolog);
      delete[] infolog;
    }

  GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentShader, NULL);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &state);
  if(state == GL_FALSE)
    {
      glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &log_len);
      GLchar *infolog = new GLchar[log_len];
      glGetShaderInfoLog(fragment, log_len, NULL, infolog);
      fprintf(stderr, "Fragment shader failed, %s\n", infolog);
      delete[] infolog;
    }

  GLuint program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &state);
  if(state == GL_FALSE)
    {
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
      GLchar *infolog = new GLchar[log_len];
      glGetProgramInfoLog(program, log_len, NULL, infolog);
      fprintf(stderr, "Program shader link failed, %s\n", infolog);
      delete[] infolog;
    }

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return program;
}


/*!
 * \brief RenderText::getCharFromFloat
 * \param number vector where char numbers will be stored
 * \param input float value to divide by char numbers
 *
 */
void RenderText::getCharFromFloat(std::vector<char> *number, double input)
{
  double frac_part = 0;
  double int_part = 0;
  int offs_begin = 0;
  int offs_end = 0;

  number->reserve(MAX_CH);

  frac_part = modf((input), &int_part);

  if( (int_part < 0) || (frac_part < 0) )
    {
      number->push_back('-');
      offs_begin++;
      int_part *= -1;
      frac_part *= -1;
    }

  if(int_part == 0)
    {
      number->push_back('0');
    }

  double add_int_part = 0;
  char to_vector;
  while (int_part > 0)
    {
      add_int_part = modf((int_part / 10), &int_part);
      add_int_part *= 10;
      to_vector = static_cast<char>(char('0') + (add_int_part + 0.000001));
      number->push_back(to_vector);
    }

  if(frac_part > 0)
    {
      number->push_back(',');
      ++offs_end;
    }
  else
    {
      std::reverse((number->begin() + offs_begin), number->end());
      return;
    }

  frac_part += 0.00000001;
  double add_frac_part = 0;
  double eps = 0.0000001;
  while ( frac_part > 0)
    {
      frac_part = modf((frac_part * 10), &add_frac_part);
      to_vector = static_cast<char>(char('0')  + (add_frac_part));
      number->push_back(to_vector);
      ++offs_end;
      eps *= 10;
      if((frac_part < eps) || (offs_end > MAX_CH - 2))
        {
          break;
        }
    }

  std::reverse((number->begin() + offs_begin), (number->end() - offs_end));
}


void RenderText::setTextY(std::vector<double> &y)
{
  textBoxesY.clear();
  textBoxesY.resize(y.size());

  textMaxWidth = 0;
  int max = 0;


  for(size_t i = 0; i < y.size(); i++)
    {
      textBoxesY[i].num = y[i];
      getCharFromFloat(&textBoxesY[i].print, textBoxesY[i].num);

      textBoxesY[i].printInfo.resize(textBoxesY[i].print.size());
      textBoxesY[i].pos.resize(textBoxesY[i].print.size() * 6 * 4);

      textBoxesY[i].width = textBoxesY[i].print.size() * characterWidth;

      max = textBoxesY[i].width;
      if(max > textMaxWidth)
        {
          textMaxWidth = max;
        }

      for(size_t j = 0; j < textBoxesY[i].print.size(); j++)
        {
          textBoxesY[i].printInfo[j].Advance =
              Characters.at(textBoxesY[i].print[j]).Advance;

          textBoxesY[i].printInfo[j].Bearing =
              Characters.at(textBoxesY[i].print[j]).BearingX;

          textBoxesY[i].printInfo[j].texX =
              Characters.at(textBoxesY[i].print[j]).texX;
        }
    }
}


void RenderText::setTextX(std::vector<double> &x)
{
  textBoxesX.clear();
  textBoxesX.resize(x.size());
  textHeight = characterHeight;

  for(size_t i = 0; i < x.size(); i++)
    {
      textBoxesX[i].num = x[i];
      getCharFromFloat(&textBoxesX[i].print, textBoxesX[i].num);

      textBoxesX[i].printInfo.resize(textBoxesX[i].print.size());
      textBoxesX[i].pos.resize(textBoxesX[i].print.size() * 6 * 4);

      textBoxesX[i].width = textBoxesX[i].print.size() * characterWidth;

      for(size_t j = 0; j < textBoxesX[i].print.size(); j++)
        {
          textBoxesX[i].printInfo[j].Advance =
              Characters.at(textBoxesX[i].print[j]).Advance;

          textBoxesX[i].printInfo[j].Bearing =
              Characters.at(textBoxesX[i].print[j]).BearingX;

          textBoxesX[i].printInfo[j].texX =
              Characters.at(textBoxesX[i].print[j]).texX;
        }
    }
}


void RenderText::updateTextPositionsX()
{
  double xpos = 0;
  double ypos = 0;
  double x_hinted = 0;
  double xpos_hinted = 0;
  double ypos_hinted = 0;
  double tex_pos_x = 0;
  double char_width = characterWidth * pixelWidth;
  double char_height = characterHeight * pixelHeight;

  for(size_t i = 0; i < textBoxesX.size(); i++ )
    {
      xpos = textBoxesX[i].num - (textBoxesX[i].width * pixelWidth / 2);
      ypos = proj.bottom + (pixelHeight * 1);

      x_hinted = hintToPixel(xpos, pixelWidth);
      ypos_hinted = hintToPixel(ypos, pixelHeight);

      for (size_t j = 0; j < textBoxesX[i].pos.size(); j+=24)
        {
          xpos_hinted = x_hinted +
              ( textBoxesX[i].printInfo[j/24].Bearing * pixelWidth );

          tex_pos_x = textBoxesX[i].printInfo[j/24].texX;

          textBoxesX[i].pos[j] = xpos_hinted;
          textBoxesX[i].pos[j + 1] = ypos_hinted + char_height;
          textBoxesX[i].pos[j + 2] = tex_pos_x;
          textBoxesX[i].pos[j + 3] = 0;

          textBoxesX[i].pos[j + 4] = xpos_hinted;
          textBoxesX[i].pos[j + 5] = ypos_hinted;
          textBoxesX[i].pos[j + 6] = tex_pos_x;
          textBoxesX[i].pos[j + 7] = 1.0;

          textBoxesX[i].pos[j + 8] = xpos_hinted + char_width;
          textBoxesX[i].pos[j + 9] = ypos_hinted;
          textBoxesX[i].pos[j + 10] = tex_pos_x + texAtlas.colFactor;
          textBoxesX[i].pos[j + 11] = 1.0;

          textBoxesX[i].pos[j + 12] = xpos_hinted;
          textBoxesX[i].pos[j + 13] = ypos_hinted + char_height;
          textBoxesX[i].pos[j + 14] = tex_pos_x;
          textBoxesX[i].pos[j + 15] = 0;

          textBoxesX[i].pos[j + 16] = xpos_hinted + char_width;
          textBoxesX[i].pos[j + 17] = ypos_hinted;
          textBoxesX[i].pos[j + 18] = tex_pos_x + texAtlas.colFactor;
          textBoxesX[i].pos[j + 19] = 1.0;

          textBoxesX[i].pos[j + 20] = xpos_hinted + char_width;
          textBoxesX[i].pos[j + 21] = ypos_hinted + char_height;
          textBoxesX[i].pos[j + 22] = tex_pos_x + texAtlas.colFactor;
          textBoxesX[i].pos[j + 23] = 0;

          x_hinted += textBoxesX[i].printInfo[j/24].Advance * pixelWidth;
        }
    }
}


void RenderText::updateTextPositionsY()
{
  double xpos = 0;
  double ypos = 0;
  double x_hinted = 0;
  double xpos_hinted = 0;
  double ypos_hinted = 0;
  double tex_pos_x = 0;
  double char_width = characterWidth * pixelWidth;
  double char_height = characterHeight * pixelHeight;

  for(size_t i = 0; i < textBoxesY.size(); i++ )
    {
      xpos = proj.left + (pixelWidth * 4);
      ypos = textBoxesY[i].num - (char_height / 2);

      x_hinted = hintToPixel(xpos, pixelWidth);
      ypos_hinted = hintToPixel(ypos, pixelHeight);

      for (size_t j = 0; j < textBoxesY[i].pos.size(); j+=24)
        {
          xpos_hinted = x_hinted +
              ( textBoxesY[i].printInfo[j/24].Bearing * pixelWidth );

          tex_pos_x = textBoxesY[i].printInfo[j/24].texX;

          textBoxesY[i].pos[j] = xpos_hinted;
          textBoxesY[i].pos[j + 1] = ypos_hinted + char_height;
          textBoxesY[i].pos[j + 2] = tex_pos_x;
          textBoxesY[i].pos[j + 3] = 0;

          textBoxesY[i].pos[j + 4] = xpos_hinted;
          textBoxesY[i].pos[j + 5] = ypos_hinted;
          textBoxesY[i].pos[j + 6] = tex_pos_x;
          textBoxesY[i].pos[j + 7] = 1.0;

          textBoxesY[i].pos[j + 8] = xpos_hinted + char_width;
          textBoxesY[i].pos[j + 9] = ypos_hinted;
          textBoxesY[i].pos[j + 10] = tex_pos_x + texAtlas.colFactor;
          textBoxesY[i].pos[j + 11] = 1.0;

          textBoxesY[i].pos[j + 12] = xpos_hinted;
          textBoxesY[i].pos[j + 13] = ypos_hinted + char_height;
          textBoxesY[i].pos[j + 14] = tex_pos_x;
          textBoxesY[i].pos[j + 15] = 0;

          textBoxesY[i].pos[j + 16] = xpos_hinted + char_width;
          textBoxesY[i].pos[j + 17] = ypos_hinted;
          textBoxesY[i].pos[j + 18] = tex_pos_x + texAtlas.colFactor;
          textBoxesY[i].pos[j + 19] = 1.0;

          textBoxesY[i].pos[j + 20] = xpos_hinted + char_width;
          textBoxesY[i].pos[j + 21] = ypos_hinted + char_height;
          textBoxesY[i].pos[j + 22] = tex_pos_x + texAtlas.colFactor;
          textBoxesY[i].pos[j + 23] = 0;

          x_hinted += textBoxesY[i].printInfo[j/24].Advance * pixelWidth;
        }
    }
}


/*!
 * \brief RenderText::genTextures
 *
 * EN
 *
 * RU
 */
void RenderText::genTextures()
{
  QString c = "0123456789,-e";
  createQCharacters(c);
}


/*!
 * \brief Plot::createQCharacter
 * \param ch char letter to load to texture or ASCII code of it
 *
 * EN
 *
 * RU Генерация атласа текстур текстовых символов.
 * Выходные текстуры символов одинакового размера, поэтому
 * для отрисовки важно учитывать сдвиг от левого
 * края. QPainter рисует текстуры в инверсном
 * цвете, так как для экономии ресурсов используется
 * только красный канал и он должен равняться 1 для
 * отрисовки текстуры.
 */
void RenderText::createQCharacters(QString &q_str)
{
  Characters.reserve(q_str.length());
  QFont qfont;

  qfont.setStyleStrategy(QFont::PreferAntialias);
  qfont.setPointSize(9);
//  qfont.setPixelSize(12);

  QFontMetrics qftmetrics(qfont);

  uint width =  qftmetrics.horizontalAdvance(q_str[0]);
  uint height = qftmetrics.height();

  characterWidth = width;
  characterHeight = height;

  QImage qimg(width * q_str.size(), height, QImage::Format_Grayscale8);

  QPainter qpaint(&qimg);
  qpaint.setFont(qfont);
  qpaint.setBrush(Qt::white);
  qpaint.setPen(Qt::white);
  qpaint.setRenderHint(QPainter::TextAntialiasing, true);
  qpaint.setRenderHint(QPainter::SmoothPixmapTransform, true);
  qpaint.fillRect(0, 0, width * q_str.size(), height, Qt::black);

  texAtlas.cols = q_str.size();
  texAtlas.rows = 1;
  texAtlas.colFactor = 1/static_cast<GLdouble>(texAtlas.cols);
  texAtlas.rowFactor = 1/static_cast<GLdouble>(texAtlas.rows);

  int curr_row = 0;
  for(QString::Iterator c = q_str.begin(); c != q_str.end(); c++)
    {
      char ch = QString(*c).toLocal8Bit().front();
      qpaint.drawText((curr_row * width) - qftmetrics.leftBearing(*c),
                      height - qftmetrics.descent(), (*c));

      Character character = {
        width,
        height,
        qftmetrics.leftBearing(*c),
        qftmetrics.ascent(),
        qftmetrics.horizontalAdvance(*c),
        (curr_row * texAtlas.colFactor),
        0.0,
      };

      Characters.insert(std::pair<char, Character>(ch, character));
      curr_row++;
    }

  glGenTextures(1, &Texture);
  glBindTexture(GL_TEXTURE_2D, Texture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        width * q_str.size(),
        height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        qimg.constBits());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, 0);
}

