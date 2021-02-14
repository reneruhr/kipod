#pragma once
#include "../core.h"
#include "render_texture.h"
#include "render_framebuffer.h"

namespace kipod{

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
