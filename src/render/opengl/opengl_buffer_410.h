#pragma once
#include "opengl_buffer.h"
namespace kipod{
class VertexBuffer410 : public VertexBuffer
{
public:
    VertexBuffer410() = default;
    VertexBuffer410(void* data, unsigned int count, unsigned int size, GLenum  flag = GL_STATIC_DRAW);
    void Set(GLenum  flag = GL_STATIC_DRAW);
    virtual void Add(unsigned int offset, unsigned long size, const void *data) override;
    virtual void Add(unsigned long size, const void *data) override;
};
}
