#include "opengl_buffer_450.h"

kipod::VertexBuffer450::VertexBuffer450(void *data, unsigned int size) : VertexBuffer(data, 0, size) {
    Set();
}
kipod::VertexBuffer450::VertexBuffer450(void* data, unsigned int size, GLenum flag) : VertexBuffer(data, 0, size) {
    Set(flag);
}

void kipod::VertexBuffer450::Set(GLenum flag)
{
    LOG_ENGINE("Setting VertexBuffer OpenGL 450");
    glCreateBuffers(1, &this->id_);
    glNamedBufferStorage(this->id_, size_, data_, flag);
}

void kipod::VertexBuffer450::Add(unsigned int offset, unsigned long size, const void *data){
    glNamedBufferSubData(this->id_, offset, size, data);
}

void kipod::VertexBuffer450::Add(unsigned long size, const void *data){
    glNamedBufferSubData(this->id_, current_buffer_offset_, size, data);
    current_buffer_offset_+=size;
}
