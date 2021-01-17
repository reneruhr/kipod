#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H


#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <vector>
#include <string>




class IBuffer{
    public:
    unsigned int id_;
    unsigned int size_;
    unsigned int count_;
};




template <typename T>
class Buffer : public IBuffer{
protected:
    std::vector<T> data_;
public:
    virtual void Bind()=0;
};




class ElementsBuffer : public Buffer<unsigned int>
{
public:
    void Set()
    {
        glGenBuffers(1, &id_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_, data_.data(), GL_STATIC_DRAW);
    }

    void Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }

    void Unbind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};




template <typename T>
class VertexBuffer : public Buffer<T>
{
public:
    void Set()
    {
        glGenBuffers(1, &this->id_);
        glBindBuffer(GL_ARRAY_BUFFER, this->id_);
        glBufferData(GL_ARRAY_BUFFER, this->size_, this->data_.data(), GL_STATIC_DRAW);

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
protected:
    GLenum type_;
    GLboolean normalized_;
    GLsizei stride_;
    std::size_t offset;

public:

    void Bind()
    {
        glVertexAttribPointer(this->id_, this->size_, type_, normalized_, stride_, (void *)offset);
        glEnableVertexAttribArray(this->id_);
    }
};




template <typename T>
class VertexAttributeObject : public IBuffer
{
    std::vector<Attribute> attributes_;
    VertexBuffer<T>* vbo;


public:

    void Add(Attribute attribute)
    {
        attributes_.push_back(attribute);
    }

    void Set()
    {
        glGenVertexArrays(1, &this->id_);
        glBindVertexArray(this->id_);

        vbo->Set();
        for(auto& a : attributes_)
            a.Bind();
    }

    void Bind(){
        glBindVertexArray(this->id_);
        vbo->Bind();
    }

    void Unbind(){
        vbo->Unbind();
        glBindVertexArray(0);
    }
};

#endif // RENDER_BUFFER_H
