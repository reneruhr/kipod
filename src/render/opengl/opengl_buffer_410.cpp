#include "opengl_buffer_410.h"

kipod::VertexBuffer410::VertexBuffer410(void *data, unsigned int count, unsigned int size, GLenum flag) : VertexBuffer(data, count, size) {
    Set(flag);
}

void kipod::VertexBuffer410::Set(GLenum flag){
    LOG_ENGINE("Setting VertexBuffer OpenGL 410");
    glGenBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size_, data_, flag);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void kipod::VertexBuffer410::Add(unsigned int offset, unsigned long size, const void *data)
{
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void kipod::VertexBuffer410::Add(unsigned long size, const void *data)
{
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferSubData(GL_ARRAY_BUFFER, current_buffer_offset_, size, data);
    current_buffer_offset_+=size;
}
