#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "render_object.h"
#include "render_light.h"
#include "render_camera.h"
#include <vector>


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
