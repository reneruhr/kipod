#ifndef RENDERENGINE_H
#define RENDERENGINE_H

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



namespace kipod{

class RenderEngine
{
public:
    RenderEngine() {}

    void Setup(RenderObject* object);
    void Draw(RenderObject* object);

};





}


#endif // RENDERENGINE_H
