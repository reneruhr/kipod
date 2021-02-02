#ifndef RENDERPRIMITIVE_H
#define RENDERPRIMITIVE_H

#include <glm/matrix.hpp>

namespace kipod
{

enum RenderObjectType
{
    RENDER_WIREFRAME   = 1 << 0,
    RENDER_VERTEX      = 1 << 1,
    RENDER_NORMALS     = 1 << 2,
    RENDER_TEXTURE     = 1 << 3,
    RENDER_DEPTH       = 1 << 4,
    RENDER_Dim2        = 1 << 5
};



class RenderPrimitive
{
public:
    RenderPrimitive();
    int size_;
};


}
#endif // RENDERPRIMITIVE_H
