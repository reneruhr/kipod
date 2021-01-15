#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <vector>
#include <string>



namespace kipod
{


class IUniform{
protected:
    std::string name_;

    IUniform(std::string& name) : name_(name){};

    virtual void Update(glm::vec4& v);
    virtual void Set(glm::vec3& v);
    virtual void Set(glm::vec2& v);
    virtual void Set(glm::mat4& v);

};




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
    void Bind()
    {
        glGenBuffers(1, &id_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_, data_.data(), GL_STATIC_DRAW);
    }

    void Unbind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};



template <typename T>
class VertexBuffer : public Buffer<T>
{
public:
    void Bind()
    {
        glGenBuffers(1, &this->id_);
        glBindBuffer(GL_ARRAY_BUFFER, this->id_);
        glBufferData(GL_ARRAY_BUFFER, this->size_, this->data_.data(), GL_STATIC_DRAW);

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
class VertexAttributeObject
{
    std::vector<Attribute> attributes_;
    VertexBuffer<T>* vbo;
    ElementsBuffer* ebo;

protected:

    void Add(Attribute attribute)
    {
        attributes_.push_back(attribute);
    }

    void Bind()
    {
        ebo->Bind();

        glGenVertexArrays(1, &this->id_);
        glBindVertexArray(this->id_);

        vbo->Bind();
        for(auto& a : attributes_)
            a.Bind();
    }

    void Unbind()
    {
        ebo->Unbind();
        vbo->Unbind();
        glBindVertexArray(0);
    }

    void QuickBind(){
        glBindVertexArray(this->id_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo->id_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id_);
    }

    void QuickUnBind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id_);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};




template <typename T>
class Uniform : public IUniform
{
    T data_;

    GLuint program_;
    GLint location_;
    Uniform(std::string name_, GLuint program) : IUniform(name_), program_(program){
        location_ = GetLocation();
    }
public:
    GLint GetLocation(){
        return  glGetUniformLocation(program_, name_.c_str());
    }


    void Set(T& data);

};





class RenderObject
{

public:
    RenderObject();

};




template <typename T>
class GLObject : public RenderObject
{
private:

    GLObject();
    VertexBuffer<T>* vbo;
    ElementsBuffer* ebo;
    VertexAttributeObject<T>* vao;
    GLuint frame_buffer_ = 0;
    GLuint program_;

    std::vector<IUniform*> uniforms_;
};


} // kipod

#endif // RENDEROBJECT_H
