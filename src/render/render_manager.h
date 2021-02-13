#pragma once
#include "../core.h"
#include "render_texture.h"

namespace kipod{

struct FrameBuffer{
    int id_;
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


class RenderManager
{
    inline static std::vector<FrameBuffer> frame_buffers_;
public:

    RenderManager();

    static FrameBuffer* addFrameBuffer(unsigned int width = 0, unsigned int height = 0);

    static void Bind(int id);

    static unsigned int* Get(int id);
};

}
