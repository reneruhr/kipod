#include "opengl_texture_410.h"
#include "opengl_texture.h"
namespace kipod
{

    void RenderToTexture410(GLuint& frame_buffer, TextureOpenGL* texture_implementation)
    {
        auto& id_ = texture_implementation->id_;
        auto& depths_id_ = texture_implementation->depths_id_;
        auto tex_ = texture_implementation->Tex();
        auto image_ = tex_->GetImage();
    	
        if (frame_buffer) {
            glDeleteFramebuffers(1, &frame_buffer);
            glDeleteTextures(1, &id_);
            glDeleteTextures(1, &depths_id_);
            id_ = 0;
            depths_id_ = 0;
        }

        glGenFramebuffers(1, &frame_buffer);



        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image_->width_, image_->height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id_, 0);


        glGenTextures(1, &depths_id_);
        glBindTexture(GL_TEXTURE_2D, depths_id_);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, image_->width_, image_->height_);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depths_id_, 0);


        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        //LOG_ENGINE("Framebuffer not complete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        LOG_ENGINE("Created Texture {} and Framebuffer {}", id_, frame_buffer);
    }

	
}