#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <vector>
#include <string>

#include "render_buffer.h"
#include "render_shader.h"
#include "render_manager.h"



enum RenderObjectType
{
    RENDER_WIREFRAME   = 1 << 0,
    RENDER_VERTEX      = 1 << 1,
    RENDER_NORMALS     = 1 << 2,
    RENDER_TEXTURE     = 1 << 3,
    RENDER_DEPTH       = 1 << 4,
    RENDER_Dim2        = 1 << 5
};

class RenderObject
{

public:
    RenderObject();
    RenderObjectType type_;
};


template <typename Primitive>
class GLObject : public RenderObject
{
private:

    GLObject();
    VertexBuffer<Primitive>* vbo;
    ElementsBuffer* ebo;
    VertexAttributeObject<Primitive>* vao;
    Shader shader_;
    std::vector<FrameBuffer> frame_buffers_;
};


#endif // RENDEROBJECT_H
