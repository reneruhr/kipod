#pragma once
#include "../core.h"

namespace kipod{
class Texture;

class FrameBuffer{
public:
    int id_ = 0;
    unsigned int opengl_id_ = 0;
    unsigned int depth_buffer_id_ = 0;
    unsigned int width_, height_;
    std::shared_ptr<Texture> tex_ = nullptr;

    FrameBuffer(unsigned int width = 0, unsigned int height = 0);
    ~FrameBuffer();

    void Bind();
    unsigned int FrameBufferAsTexture();
    void Resize(unsigned int w, unsigned int h);
};
}
