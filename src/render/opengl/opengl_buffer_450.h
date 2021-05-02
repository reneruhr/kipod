#pragma once
#include "opengl_buffer.h"
namespace kipod{
class VertexBuffer450 : public VertexBuffer
{

public:
    VertexBuffer450() = default;
    VertexBuffer450(void *data, unsigned int size);
    VertexBuffer450(void* data, unsigned int size, GLenum flag);
    void Set(GLenum flag=GL_DYNAMIC_STORAGE_BIT);
    virtual void Add(unsigned int offset, unsigned long size, const void *data) override;
    virtual void Add(unsigned long size, const void *data) override;
};
}
