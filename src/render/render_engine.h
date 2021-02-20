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
public:
    RenderEngine() {}

    void Setup(RenderObject* object);
    void Draw(RenderObject* object);

};


}
