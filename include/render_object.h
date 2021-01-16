#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <vector>
#include <string>

#include "render_buffer.h"
#include "render_shader.h"
#include "render_manager.h"

#include "texture.h"
#include <render_primitive.h>


class RenderObject
{

public:
    RenderObject();
    RenderObjectType type_;
};


template <typename Primitive>
class GLObject : public RenderObject
{
public:

    GLObject();
    VertexBuffer<Primitive>* vbo_;
    ElementsBuffer* ebo_;
    VertexAttributeObject<Primitive>* vao_;
    Shader sha_;
    std::vector<FrameBuffer> fra_;
    Texture* tex_;

};




#endif // RENDEROBJECT_H
