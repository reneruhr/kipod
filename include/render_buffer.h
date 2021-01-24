#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H


#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <vector>
#include <string>

#include "utils/log.h"

namespace kipod{

class IBuffer{
    public:
    IBuffer() = default;
    IBuffer(unsigned int count, unsigned int size) : count_(count), size_(size){}
    virtual ~IBuffer() = default;
    unsigned int id_;
    unsigned int count_;
    unsigned int size_;
};





class Buffer : public IBuffer{
protected:
    void* data_ = nullptr;

public:
    Buffer() = default;
    Buffer(void* data, unsigned int count, unsigned int size) : IBuffer(count, size), data_(data){}
    virtual ~Buffer() = default;
    virtual void Bind()=0;
};




class ElementsBuffer : public Buffer
{
public:
    ElementsBuffer() = default;
    ElementsBuffer(void* data, unsigned int count, unsigned int size) : Buffer(data, count, size) {}
    void Set()
    {
        glGenBuffers(1, &id_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_, data_, GL_STATIC_DRAW);
        LOG_ENGINE("Setting ElementsBuffer with id {}", id_);
    }

    void Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }

    void Unbind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};



class VertexBuffer : public Buffer
{
public:
    VertexBuffer() = default;
    VertexBuffer(void* data, unsigned int size) : Buffer(data, 0, size) { Set(); }

    void Set()
    {
        LOG_ENGINE("Setting VertexBuffer");
        glCreateBuffers(1, &this->id_);
        glNamedBufferStorage(this->id_, size_, data_, GL_DYNAMIC_STORAGE_BIT);
        //glBufferData(GL_ARRAY_BUFFER, this->size_, this->data_, GL_STATIC_DRAW);
    }

    void Add(unsigned int offset, unsigned int size, const void* data){
        glNamedBufferSubData(this->id_, offset, size, data);
    }

    void Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->id_);
    }

    void Unbind(){
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};




class Attribute : public IBuffer
{
public:
    GLenum type_ = GL_FLOAT;
    GLboolean normalized_ = GL_FALSE;
    GLsizei stride_ = 0;
    std::size_t offset_ = 0;
public:
    //Attribute() = default;
    Attribute(unsigned int id = 0, unsigned int count = 0, GLsizei stride = 0, std::size_t offset = 0 ) :
         stride_(stride), offset_(offset) { count_ = count; id_ = id;}

    void Set()
    {
        LOG_ENGINE("Setting Attribute {}. Count: {}. Type: {}. Stride: {}. Offset: {}",id_, count_, type_, stride_, offset_);
        glVertexAttribPointer(this->id_, this->count_, type_, normalized_, stride_, (void *)offset_);
        glEnableVertexAttribArray(this->id_);
    }
};



class VertexAttributeObject : public IBuffer
{
    std::vector<Attribute*> attributes_;

public:


    void Add(Attribute* attribute)
    {
        attributes_.push_back(attribute);
    }

    void Set()
    {
        glGenVertexArrays(1, &this->id_);
        glBindVertexArray(this->id_);
    }

    void SetAttributes()
    {
        for(auto& a : attributes_)  a->Set();
    }

    void Bind(){
        glBindVertexArray(this->id_);
    }

    void Unbind(){
        glBindVertexArray(0);
    }
};


}
#endif // RENDER_BUFFER_H
