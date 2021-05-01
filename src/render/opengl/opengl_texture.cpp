#include "opengl_texture.h"
#include "opengl_layout.h"
#include "opengl_engine.h"
#include "opengl_texture_410.h"
#include "opengl_texture_450.h"

void kipod::TextureOpenGL::SetupTextureToSquare(float height)
{
    dynamic_cast<GLRenderLayout*>(tex_->textured_square_->Layout())->sha_ = std::make_shared<Shader>("passthrough.vert.glsl", "passthrough.frag.glsl");
    dynamic_cast<GLRenderLayout*>(tex_->textured_square_->Layout())->sha_->AttachUniform<int>("tex");
}

void kipod::TextureOpenGL::Draw()
{

}

void kipod::TextureOpenGL::Resize(int w, int h)
{

}

void kipod::TextureOpenGL::LoadTexture(int texture_option_filter, int texture_option_wrap){

       glGenTextures(1, &id_);
       glBindTexture(GL_TEXTURE_2D, id_);

       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_option_wrap);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_option_wrap);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_option_filter);
       //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_option_filter);

       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_->image_->width_, tex_->image_->height_, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_->image_->data_);
       //glGenerateMipmap(GL_TEXTURE_2D);
}

//
//void kipod::TextureOpenGL::RenderToTexture(GLuint& frame_buffer)
//{
//        if(frame_buffer){
//            glDeleteFramebuffers(1, &frame_buffer);
//            glDeleteTextures(1, &id_);
//            glDeleteTextures(1, &depths_id_);
//            id_=0;
//            depths_id_=0;
//        }
//
//        glGenFramebuffers(1, &frame_buffer);
//        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
//
//        glGenTextures(1, &id_);
//        glBindTexture(GL_TEXTURE_2D, id_);
//
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_->image_->width_, tex_->image_->height_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//        glGenRenderbuffers(1, &depths_id_);
//        glBindRenderbuffer(GL_RENDERBUFFER, depths_id_);
//        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, tex_->image_->width_, tex_->image_->height_);
//        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depths_id_);
//
//        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, id_, 0);
//
//        GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
//        glDrawBuffers(1, draw_buffers);
//
//        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//            LOG_ENGINE("Failed to Render to Texture");
//
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}



kipod::TextureOpenGL::~TextureOpenGL(){
    glDeleteTextures(1, &depths_id_);
    glDeleteTextures(1, &id_);
}

void kipod::TextureOpenGL::Bind() const {
    glBindTexture(GL_TEXTURE_2D, id_);
}

void kipod::TextureOpenGL::RenderToTexture(GLuint& frame_buffer)
{
    if (OpenGLEngine::Version() == 450)           RenderToTexture450(frame_buffer, this);
    else                                          RenderToTexture410(frame_buffer, this);
}


