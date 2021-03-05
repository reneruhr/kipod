#pragma once
#include "../render_buffer.h"

namespace kipod{
class ElementsBuffer : public Buffer
{
public:
    ElementsBuffer() = default;
    ElementsBuffer(const ElementsBuffer& elementbuffer) = default;
    ElementsBuffer(void* data, unsigned int count, unsigned int size, GLchar primitive=GL_TRIANGLES) : Buffer(data, count, size), primitive_(primitive) {}
    GLchar primitive_;
    void Set();
    void Bind();
    void Unbind();

};

class Attribute : public IBuffer
{
    GLenum type_ = GL_FLOAT;
    GLboolean normalized_ = GL_FALSE;
    GLsizei stride_ = 0;
    std::size_t offset_ = 0;
public:
    Attribute(unsigned int id = 0, unsigned int count = 0, GLsizei stride = 0, std::size_t offset = 0 );
    void Set();
};


class VertexAttributeObject : public IBuffer
{
    std::vector<Attribute> attributes_;
public:
    unsigned short NumberOfAttributes() { return size(attributes_); }
    void Add(Attribute&& attribute);
    void Set();
    void SetAttributes();
    void Bind();
    void Unbind();
};

class VertexBuffer : public Buffer
{
protected:
        unsigned long current_buffer_offset_ = 0;
public:
    VertexBuffer() = default;
    VertexBuffer(void *data, unsigned int count, unsigned int size) :  Buffer(data, count, size) {}
    virtual void Bind();
    virtual void Unbind();
    virtual void Add(unsigned int , unsigned long , const void*){}
    virtual void Add(unsigned long, const void*){}
    virtual ~VertexBuffer() = default;
};
}

