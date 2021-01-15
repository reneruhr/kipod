#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <vector>
#include "GL/glew.h"

struct FrameBuffer{
    int id_;
    unsigned int opengl_id_ = 0;

    FrameBuffer(){
        static int n_frame_buffers = 0;
        id_ = n_frame_buffers++;
    }

    void Bind(){
        glBindFramebuffer(GL_FRAMEBUFFER, opengl_id_);
    }
};

class RenderManager
{
    inline static std::vector<FrameBuffer> frame_buffers_;
public:

    RenderManager();

//    static void Init(){
//        frame_buffers_={};
//    }

    static FrameBuffer* addFrameBuffer(){
        frame_buffers_.emplace_back(FrameBuffer());
        return &frame_buffers_.back();
    }

    static void Bind(int id){
        frame_buffers_[id].Bind();
    }

    static unsigned int* Get(int id){
        return &frame_buffers_[id].opengl_id_;
    }
};

#endif // RENDERMANAGER_H
