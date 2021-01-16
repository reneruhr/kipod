#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "render_object.h"

#include <vector>

class RenderEngine
{
public:
    RenderEngine();

    std::vector<Shader*> shaders_;

    template <typename Primitive>
        void Setup(RenderObject* object);

    template <typename Primitive>
        void Draw(RenderObject* object);

};








#endif // RENDERENGINE_H
