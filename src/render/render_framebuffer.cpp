#include "render_framebuffer.h"
#include "render_texture.h"

kipod::FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) : width_(width), height_(height)
{
    static int n_frame_buffers = 0;
    id_ = n_frame_buffers++;
    if(width_ && height_){
        LOG_ENGINE("Create Textured FrameBuffer (count ={}) with Width {}, Height {}", id_, width_, height_);
        tex_ = std::make_shared<Texture>(width_, height_);
        tex_->RenderToTexture(opengl_id_);
        tex_->SetupTextureToSquare();
    } else
        LOG_ENGINE("Created Empty FrameBuffer (count ={}), opengl id {}", id_, opengl_id_);
}

kipod::FrameBuffer::~FrameBuffer(){
    glDeleteFramebuffers(1, &opengl_id_);
}

void kipod::FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, opengl_id_);
}

unsigned int kipod::FrameBuffer::FrameBufferAsTexture(){
    return tex_->id_;
}

void kipod::FrameBuffer::Resize(unsigned int w, unsigned int h){
//    width_ =w; height_ =h;

//    auto new_tex = std::make_shared<Texture>(width_, height_);

//    unsigned int new_opengl_id;
//    new_tex->RenderToTexture(new_opengl_id);
//    std::swap(opengl_id_, new_opengl_id);
//    glDeleteFramebuffers(1, &new_opengl_id);
}
