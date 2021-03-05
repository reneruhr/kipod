#pragma once
//#include "opengl_window.h"
#include "../render_window.h"
namespace kipod{

class OpenGLEngine
{
    OpenGLEngine(){}
    inline static Window* window_ = nullptr;

public:
    OpenGLEngine(OpenGLEngine&) = delete;
    void operator=(OpenGLEngine&)  = delete;
    ~OpenGLEngine() = default;
    static OpenGLEngine& Get(){  static OpenGLEngine OpenGLEngine;   return OpenGLEngine;  }
    static void SetWindow(Window* window) {  Get().window_ = window; }
    static int Version() { return Get().window_->opengl_version_; }

};

}
