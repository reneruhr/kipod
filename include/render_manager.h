#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <core.h>
#include <render_texture.h>

namespace kipod{

struct FrameBuffer{
    int id_;
    unsigned int opengl_id_ = 0;

    unsigned int width_, height_;

    std::shared_ptr<Texture> tex_ = nullptr;

    FrameBuffer(unsigned int width = 0, unsigned int height = 0) : width_(width), height_(height)
    {
        static int n_frame_buffers = 0;
        id_ = n_frame_buffers++;
        if(width_ && height_){
            LOG_ENGINE("Create Textured FrameBuffer (count ={}) with Width {}, Height {}", id_, width_, height_);
            tex_ = std::make_shared<Texture>(width_, height_);
            tex_->RenderToTexture2(opengl_id_);
        } else
            LOG_ENGINE("Created Empty FrameBuffer (count ={}), opengl id {}", id_, opengl_id_);
    }

    ~FrameBuffer(){
        glDeleteFramebuffers(1, &opengl_id_);
    }

    void Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, opengl_id_);
    }

    unsigned int FrameBufferAsTexture(){
        return tex_->id_;
    }

    void Resize(unsigned int w, unsigned int h){
        //delete tex_;

        width_ =w; height_ =h;

        auto new_tex = std::make_shared<Texture>(width_, height_);

        unsigned int new_opengl_id;
        new_tex->RenderToTexture2(new_opengl_id);
        std::swap(opengl_id_, new_opengl_id);
        glDeleteFramebuffers(1, &new_opengl_id);
    }

};


class RenderManager
{
    inline static std::vector<FrameBuffer> frame_buffers_;
public:

    RenderManager();

    static FrameBuffer* addFrameBuffer(unsigned int width = 0, unsigned int height = 0)
    {
        frame_buffers_.emplace_back(FrameBuffer(width, height));
        return &frame_buffers_.back();
    }

    static void Bind(int id)
    {
        frame_buffers_[id].Bind();
    }

    static unsigned int* Get(int id)
    {
        return &frame_buffers_[id].opengl_id_;
    }
};

}
#endif // RENDERMANAGER_H
