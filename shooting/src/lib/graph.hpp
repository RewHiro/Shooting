
#pragma once

//
// グラフィック関連
//

#include "defines.hpp"
#include "texture.hpp"
#include "vector.hpp"


class Color {
  float red_;
  float green_;
  float blue_;
  float alpha_;

  
public:  
  Color();

  // 値を0.0〜1.0で指定
  Color(const float red, const float green, const float blue,
        const float alpha = 1.0f);

  
  // 色の変更
  float& red();
  float& green();
  float& blue();
  float& alpha();


  // OpenGLへ描画色を指定
  void setToGl() const;

};


// 色を0~255で指定する
Color color256(const int red, const int green, const int blue, const int alpha = 255);

// 色をHSBで指定
// hue, saturation, brightness [0, 1.0]
Color colorHSB(const float hue, const float saturation, const float brightness);


// 点を描画
// x, y   位置
// radius 半径
// color  色
void drawPoint(const float x, const float y,
               const float radius,
               const Color& color);

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
               const Vec2f& origin);

// 線を描画
// start_x, start_y 始点
// end_x, end_y     終点
// line_width       線幅
// color            色
void drawLine(const float start_x, const float start_y,
              const float end_x, const float end_y,
              const float line_width,
              const Color& color);

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
              const Vec2f& origin);

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
                const Color& color);

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
                const Vec2f& origin);

// 塗り潰し円
// center_x, center_y 円の中心位置
// radius_x, radius_y 半径(横と縦)
// division           円の分割数(数値が大きいと滑らかな円になる)
// color              色
void drawFillCircle(const float center_x, const float center_y,
                    const float radius_x, const float radius_y,
                    const int division,
                    const Color& color);

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
                    const Vec2f& origin);

// 矩形
// start_x, start_y 始点
// width, height    幅、高さ
// line_width       線幅
// color            色
void drawBox(const float start_x, const float start_y,
             const float width, const float height,
             const float line_width,
             const Color& color);

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
             const Vec2f& origin);

// 塗り潰し矩形
// start_x, start_y 始点
// width, height    幅、高さ
// color            色
void drawFillBox(const float start_x, const float start_y,
                 const float width, const float height,
                 const Color& color);

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
                 const Vec2f& origin);

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
                    const Color& color);

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
                    const Vec2f& origin);
