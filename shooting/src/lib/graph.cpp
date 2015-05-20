
//
// グラフィック関連
//

#include "graph.hpp"
#include <vector>
#include <GLFW/glfw3.h>
#include "matrix.hpp"


Color::Color() :
  red_(0.0f),
  green_(0.0f),
  blue_(0.0f),
  alpha_(1.0f)
{}

// 値を0.0〜1.0で指定
Color::Color(const float red, const float green, const float blue,
             const float alpha) :
  red_(red),
  green_(green),
  blue_(blue),
  alpha_(alpha)
{}

  
// 色の変更
float& Color::red() { return red_; }
float& Color::green() { return green_; }
float& Color::blue() { return blue_; }
float& Color::alpha() { return alpha_; }


// OpenGLへ描画色を指定
void Color::setToGl() const {
  glColor4f(red_, green_, blue_, alpha_);
}


// 色を0~255で指定する
Color color256(const int red, const int green, const int blue, const int alpha) {
  return Color(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}

// 色をHSBで指定
// hue, saturation, brightness [0, 1.0]
Color colorHSB(const float hue, const float saturation, const float brightness) {
  float r = brightness;
  float g = brightness;
  float b = brightness;
  
  if (saturation > 0.0) {
    float h = hue * 6.0;
    int   i = h;
    float f = h - i;
    switch (i) {
    default:
    case 0:
      g *= 1.0 - saturation * (1.0 - f);
      b *= 1.0 - saturation;
      break;

    case 1:
      r *= 1.0 - saturation * f;
      b *= 1.0 - saturation;
      break;

    case 2:
      r *= 1.0 - saturation;
      b *= 1.0 - saturation * (1.0 - f);
      break;
      
    case 3:
      r *= 1.0 - saturation;
      g *= 1.0 - saturation * f;
      break;
      
    case 4:
      r *= 1.0 - saturation * (1.0 - f);
      g *= 1.0 - saturation;
      break;
      
    case 5:
      g *= 1.0 - saturation;
      b *= 1.0 - saturation * f;
      break;      
    }
  }
  
  return Color(r, g, b);
}


// 点を描画
// x, y   位置
// radius 半径
// color  色
void drawPoint(const float x, const float y,
               const float radius,
               const Color& color) {

  GLfloat vtx[] = {
    x, y
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 点の大きさ
  glPointSize(radius);
  
  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに点の描画を指示
  glDrawArrays(GL_POINTS, 0, 1);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 点を描画(回転、拡大縮小つき)
// x, y      位置
// radius    半径
// color     色
// angle_rad 回転角度(ラジアン)
// scaling   横、縦の拡大縮小率
// origin    矩形の原点位置
void drawPoint(const float x, const float y,
               const float radius,
               const Color& color,
               const float angle_rad,
               const Vec2f& scaling,
               const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(x, y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawPoint(-origin.x(), -origin.y(),
            radius,
            color);

  // 行列を元に戻す
  glPopMatrix();
}


// 線を描画
// start_x, start_y 始点
// end_x, end_y     終点
// line_width       線幅
// color            色
void drawLine(const float start_x, const float start_y,
              const float end_x, const float end_y,
              const float line_width,
              const Color& color) {

  GLfloat vtx[] = {
    start_x, start_y,
    end_x,   end_y
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 線分の太さを指示
  glLineWidth(line_width);

  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに線分の描画を指示
  glDrawArrays(GL_LINES, 0, 2);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 線を描画(回転、拡大縮小つき)
// start_x, start_y 始点
// end_x, end_y     終点
// line_width       線幅
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           矩形の原点位置
void drawLine(const float start_x, const float start_y,
              const float end_x, const float end_y,
              const float line_width,
              const Color& color,
              const float angle_rad,
              const Vec2f& scaling,
              const Vec2f& origin) {
  
  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(start_x, start_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawLine(-origin.x(), -origin.y(),
           (end_x - start_x) - origin.x(), (end_y - start_y) - origin.y(), 
           line_width,
           color);

  // 行列を元に戻す
  glPopMatrix();
}


// 円を描画
// center_x, center_y 円の中心位置
// radius_x, radius_y 半径(横と縦)
// division           円の分割数(数値が大きいと滑らかな円になる)
// line_width         線幅
// color              色
void drawCircle(const float center_x, const float center_y,
                const float radius_x, const float radius_y,
                const int division,
                const float line_width,
                const Color& color) {
  // 線分の太さを指示
  glLineWidth(line_width);

  // 色を設定
  color.setToGl();

  // 頂点データを生成
  std::vector<GLfloat> vtx;
  vtx.reserve(division * 2);
  for (int i = 0; i < division; ++i) {
    float r = (M_PI * 2.0 * i) / division;

    vtx.push_back(radius_x * std::sin(r) + center_x);
    vtx.push_back(radius_y * std::cos(r) + center_y);
  }

  glVertexPointer(2, GL_FLOAT, 0, &vtx[0]);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  
  glDrawArrays(GL_LINE_LOOP, 0, GLsizei(vtx.size() / 2));
  
  glDisableClientState(GL_VERTEX_ARRAY);
}

// 円を描画(回転、拡大縮小つき)
// center_x, center_y 円の中心位置
// radius_x, radius_y 半径(横と縦)
// division           円の分割数(数値が大きいと滑らかな円になる)
// line_width         線幅
// color              色
// angle_rad          回転角度(ラジアン)
// scaling            横、縦の拡大縮小率
// origin             矩形の原点位置
void drawCircle(const float center_x, const float center_y,
                const float radius_x, const float radius_y,
                const int division,
                const float line_width,
                const Color& color,
                const float angle_rad,
                const Vec2f& scaling,
                const Vec2f& origin) {
  
  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(center_x, center_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawCircle(-origin.x(), -origin.y(),
             radius_x, radius_y,
             line_width,
             division,
             color);

  // 行列を元に戻す
  glPopMatrix();
}


// 塗り潰し円
// center_x, center_y 円の中心位置
// radius_x, radius_y 半径(横と縦)
// division           円の分割数(数値が大きいと滑らかな円になる)
// color              色
void drawFillCircle(const float center_x, const float center_y,
                    const float radius_x, const float radius_y,
                    const int division,
                    const Color& color) {
  // 色を設定
  color.setToGl();

  // 頂点データを生成
  std::vector<GLfloat> vtx;
  vtx.reserve(division * 2);
  vtx.push_back(center_x);
  vtx.push_back(center_y);
  for (int i = 0; i <= division; ++i) {
    float r = (M_PI * 2.0 * i) / division;

    // 反時計回りが表面となる
    vtx.push_back(radius_x * -std::sin(r) + center_x);
    vtx.push_back(radius_y * std::cos(r) + center_y);
  }

  glVertexPointer(2, GL_FLOAT, 0, &vtx[0]);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  
  glDrawArrays(GL_TRIANGLE_FAN, 0, GLsizei(vtx.size() / 2));
  
  glDisableClientState(GL_VERTEX_ARRAY);
}

// 塗り潰し円(回転、拡大縮小つき)
// center_x, center_y 円の中心位置
// radius_x, radius_y 半径(横と縦)
// division           円の分割数(数値が大きいと滑らかな円になる)
// color              色
// angle_rad          回転角度(ラジアン)
// scaling            横、縦の拡大縮小率
// origin             矩形の原点位置
void drawFillCircle(const float center_x, const float center_y,
                    const float radius_x, const float radius_y,
                    const int division,
                    const Color& color,
                    const float angle_rad,
                    const Vec2f& scaling,
                    const Vec2f& origin) {
  
  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(center_x, center_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawFillCircle(-origin.x(), -origin.y(),
                 radius_x, radius_y,
                 division,
                 color);

  // 行列を元に戻す
  glPopMatrix();
}


// 矩形
// start_x, start_y 始点
// width, height    幅、高さ
// line_width       線幅
// color            色
void drawBox(const float start_x, const float start_y,
             const float width, const float height,
             const float line_width,
             const Color& color) {

  // 線分の太さを指示
  glLineWidth(line_width);

  const float end_x = start_x + width;
  const float end_y = start_y + height;

  GLfloat vtx[] = { 
    start_x, start_y,
    start_x, end_y,
    end_x,   end_y,
    end_x,   start_y,
    start_x, start_y
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに線分の描画を指示
  glDrawArrays(GL_LINE_STRIP, 0, 5);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 矩形(回転、拡大縮小つき)
// start_x, start_y 始点
// width, height    幅、高さ
// line_width       線幅
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           矩形の原点位置
void drawBox(const float start_x, const float start_y,
             const float width, const float height,
             const float line_width,
             const Color& color,
             const float angle_rad,
             const Vec2f& scaling,
             const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(start_x, start_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawBox(-origin.x(), -origin.y(),
          width, height,
          line_width,
          color);

  // 行列を元に戻す
  glPopMatrix();
}


// 塗り潰し矩形
// start_x, start_y 始点
// width, height    幅、高さ
// color            色
void drawFillBox(const float start_x, const float start_y,
                 const float width, const float height,
                 const Color& color) {

  const float end_x = start_x + width;
  const float end_y = start_y + height;
  
  GLfloat vtx[] = { 
    start_x, start_y,
    end_x,   start_y,
    start_x, end_y,
    end_x,   end_y
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);

  // 色を設定
  color.setToGl();

  glEnableClientState(GL_VERTEX_ARRAY);

  // OpenGLに線分の描画を指示
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
}

// 塗り潰し矩形(回転、拡大縮小つき)
// start_x, start_y 始点
// width, height    幅、高さ
// color            色
// angle_rad        回転角度(ラジアン)
// scaling          横、縦の拡大縮小率
// origin           矩形の原点位置
void drawFillBox(const float start_x, const float start_y,
                 const float width, const float height,
                 const Color& color,
                 const float angle_rad,
                 const Vec2f& scaling,
                 const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(start_x, start_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawFillBox(-origin.x(), -origin.y(),
              width, height,
              color);

  // 行列を元に戻す
  glPopMatrix();
}


// 画像つき矩形の描画
// start_x, start_y              始点
// width, height                 幅、高さ
// start_tx, start_ty            画像切り抜き開始位置
// texture_width, texture_height 画像切り抜き幅、高さ
// texture                       画像
// color                         色
void drawTextureBox(const float start_x, const float start_y,
                    const float width, const float height,
                    const float start_tx, const float start_ty,
                    const float texture_width, const float texture_height,
                    const Texture& texture,
                    const Color& color) {

  const float end_x = start_x + width;
  const float end_y = start_y + height;
  GLfloat vtx[] = {
    start_x, start_y,
    end_x,   start_y,
    start_x, end_y,
    end_x,   end_y
  };

  glVertexPointer(2, GL_FLOAT, 0, vtx);
  
  GLfloat uv[] = {
    start_tx / texture.width(),                   (start_ty + texture_height) / texture.height(),
    (start_tx + texture_width) / texture.width(), (start_ty + texture_height) / texture.height(),
    start_tx / texture.width(),                   start_ty / texture.height(),
    (start_tx + texture_width) / texture.width(), start_ty / texture.height(),
  };
  
  glTexCoordPointer(2, GL_FLOAT, 0, uv);
  
  // 色を設定
  color.setToGl();

  glEnable(GL_TEXTURE_2D);
  texture.bind();
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  // OpenGLに線分の描画を指示
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  
  glDisable(GL_TEXTURE_2D);
  texture.unbind();
}

// 画像つき矩形の描画(回転、拡大縮小つき)
// start_x, start_y              始点
// width, height                 幅、高さ
// start_tx, start_ty            画像切り抜き開始位置
// texture_width, texture_height 画像切り抜き幅、高さ
// texture                       画像
// color                         色
// angle_rad                     回転角度(ラジアン)
// scaling                       横、縦の拡大縮小率
// origin                        矩形の原点位置
void drawTextureBox(const float start_x, const float start_y,
                    const float width, const float height,
                    const float start_tx, const float start_ty,
                    const float texture_width, const float texture_height,
                    const Texture& texture,
                    const Color& color,
                    const float angle_rad,
                    const Vec2f& scaling,
                    const Vec2f& origin) {

  // 回転、拡大縮小の行列を生成
  auto matrix = transformMatrix2D(angle_rad,
                                  Vec3f(start_x, start_y, 0.0f),
                                  Vec3f(scaling.x(), scaling.y(), 1.0f));

  // 行列をOpenGLに設定
  glPushMatrix();
  glMultMatrixf(matrix.data());

  // 描画
  drawTextureBox(-origin.x(), -origin.y(),
                 width, height,
                 start_tx, start_ty,
                 texture_width, texture_height,
                 texture,
                 color);

  // 行列を元に戻す
  glPopMatrix();
}
