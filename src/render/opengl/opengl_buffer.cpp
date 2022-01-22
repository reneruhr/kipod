#include "opengl_buffer.h"

void kipod::ElementsBuffer::Set()
{
    glGenBuffers(1, &id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_, data_, GL_STATIC_DRAW);
    LOG_ENGINE("Setting ElementsBuffer with id {}", id_);
}

void kipod::ElementsBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

void kipod::ElementsBuffer::Unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void kipod::ElementsBuffer::SubData(unsigned int count, std::size_t start)
{
    subcount_ = count;
    start_ = start * sizeof(unsigned int);
}

void kipod::ElementsBuffer::ResetSubData()
{
    subcount_ = count_;
    start_ = 0;
}

kipod::Attribute::Attribute(unsigned int id, unsigned int count, GLsizei stride, std::size_t offset) :
    stride_(stride), offset_(offset) { count_ = count; id_ = id;}

void kipod::Attribute::Set()
{
    LOG_ENGINE("Setting Attribute {}. Count: {}. Type: {}. Stride: {}. Offset: {}",id_, count_, type_, stride_, offset_);
    glEnableVertexAttribArray(this->id_);
    glVertexAttribPointer(this->id_, this->count_, type_, normalized_, stride_, (void *)offset_);
}

void kipod::VertexAttributeObject::Add(kipod::Attribute &&attribute)
{
    attributes_.emplace_back(std::forward<Attribute>(attribute));
}

void kipod::VertexAttributeObject::Set()
{
    glGenVertexArrays(1, &this->id_);
    glBindVertexArray(this->id_);
}

void kipod::VertexAttributeObject::SetAttributes()
{
    for(auto& a : attributes_)  a.Set();
}

void kipod::VertexAttributeObject::Bind(){
    glBindVertexArray(this->id_);
}

void kipod::VertexAttributeObject::Unbind(){
    glBindVertexArray(0);
}

void kipod::VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id_);
}

void kipod::VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
