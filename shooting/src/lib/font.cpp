
//
// フォント
//

#include "font.hpp"
#include <iostream>


// 文字表示ライブラリのリンクを指示
#if defined (_MSC_VER)
#pragma comment (lib, "glu32.lib")
#ifdef _DEBUG
#pragma comment (lib, "freetyped.lib")
#pragma comment (lib, "ftgld.lib")
#else
#pragma comment (lib, "freetype.lib")
#pragma comment (lib, "ftgl.lib")
#endif
#endif


// コンストラクタ
// font_path フォントファイル(拡張子がttf)
// mode      フォント生成方式
Font::Font(const std::string& font_path, const int mode) {
  switch (mode) {
  case BUFFER:
    font_ = std::make_shared<FTBufferFont>(font_path.c_str());
    break;

  case CACHE:
    font_ = std::make_shared<FTTextureFont>(font_path.c_str());
    break;

  case POLYGON:
    font_ = std::make_shared<FTPolygonFont>(font_path.c_str());
    break;
  }
  font_->UseDisplayList(false);

  DOUT << "Font(" << font_path << ")" << std::endl;
}


// フォントサイズ指定
void Font::size(const int size) {
  font_->FaceSize(size);
}

// 描画した時のサイズを取得
Vec2f Font::drawSize(const std::string& text) {
  float llx, lly, llz;
  float urx, ury, urz;
  font_->BBox(text.c_str(), llx, lly, llz, urx, ury, urz);
  return Vec2f(urx - llx, ury - lly);
}

// 描画
// text  表示文字列
// pos   表示位置
// color 表示色
void Font::draw(const std::string& text, const Vec2f& pos, const Color& color) {
  color.setToGl();

  // OpenGLの行列スタックを利用
  glPushMatrix();
  font_->Render(text.c_str(), -1, FTPoint(pos.x(), pos.y()), FTPoint());
  glPopMatrix();
}
