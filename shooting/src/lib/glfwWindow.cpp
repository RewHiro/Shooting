
//
// GLFWのWindow管理
//

#include "glfwWindow.hpp"
#include <iostream>
#include <iostream>
#include "glExt.hpp"


// リンクするライブラリの指示(Windows)
#if defined (_MSC_VER)
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")
#endif


GlfwWindow::GlfwWindow(const int width, const int height,
                       const bool is_visible, const bool is_fullscreen) {
  DOUT << "GlfwWindow()" << std::endl;

  if (!glfwInit()) throw "Can't Initialize GLFW.";

  if (!is_visible) glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    
  window_ = glfwCreateWindow(width, height, PREPRO_TO_STR(PRODUCT_NAME),
                             is_fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
  if (!window_) {
    glfwTerminate();
    throw "Can't create GLFW window.";
  }
    
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);


  // TIPS:GLEWの初期化はglfwMakeContextCurrentの後で
  if (!initGlExt()) {
    throw "Can't use OpenGL extensions.";
  }

#if 0
  // TIPS:Windowsの古いグラフィックカードで画面の更新同期が取れないための対策
  if (isVsyncSwap()) {
    bool do_swap = VsyncSwapInterval(1);
    DOUT << "VsyncSwapInterval is " << do_swap << std::endl;
  }
  else {
    DOUT << "Can't use VsyncSwapInterval" << std::endl;
  }
#endif
    
}

GlfwWindow::~GlfwWindow() {
  DOUT << "~GlfwWindow()" << std::endl;

  glfwDestroyWindow(window_);
  glfwTerminate();
}

GLFWwindow* GlfwWindow::operator()() { return window_; }
const GLFWwindow* GlfwWindow::operator()() const { return window_; }
  
