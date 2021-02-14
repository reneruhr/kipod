#include "render_manager.h"

kipod::FrameBuffer *kipod::RenderManager::addFrameBuffer(unsigned int width, unsigned int height)
{
    frame_buffers_.emplace_back(FrameBuffer(width, height));
    return &frame_buffers_.back();
}

void kipod::RenderManager::Bind(int id)
{
    frame_buffers_[id].Bind();
}

unsigned int *kipod::RenderManager::Get(int id)
{
    return &frame_buffers_[id].opengl_id_;
}
