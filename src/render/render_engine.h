#pragma once
#include "render_object.h"
#include "render_light.h"
#include "render_window.h"
#include "render_texture.h"
#include "render_scene.h"
#include "render_shader.h"
#include "render_uniform.h"
#include "render_manager.h"
#include "render_material.h"
#include "render_primitive.h"
#include "render_camera.h"
#include "opengl/opengl_layout.h"

namespace kipod{
class RenderEngine
{
    RenderEngine(){}

    const static inline std::vector<std::string> APIs_ = {"OpenGL", "SoftRenderer", "Raytracer"};
    static inline int active_API_ = 0;

public:
    RenderEngine(RenderEngine&) = delete;
    void operator=(RenderEngine&)  = delete;
    ~RenderEngine() = default;

    static RenderEngine& Get(){  static RenderEngine RenderEngine;   return RenderEngine;  }
    static std::string ActiveAPI(){  return Get().APIs_[active_API_];  }
    static void SetAPI(std::string api){
        if(api=="OpenGL") Get().active_API_ = 0;
        else if (api=="SoftRenderer") Get().active_API_ = 1;
        else if (api=="Raytracer") Get().active_API_ = 2;
    }
};
}
