#include "opengl_texture_450.h"
#include "opengl_texture.h"

namespace kipod
{


    void LoadTextureIP450(TextureIPOpenGL* texture_implementation)
    {
        auto& id = texture_implementation->id_;
        auto tex = texture_implementation->Tex();
        auto image = tex->GetImage();
        auto buffer = texture_implementation->buffer_.get();

        glCreateTextures(GL_TEXTURE_2D, 1, &id);
        glTextureStorage2D(id, 1, GL_RGB8, image->width_, image->height_);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer->id_);
        glTextureSubImage2D(id, 
							0,
							0,0,
							image->width_, image->height_,
							GL_RGB, GL_UNSIGNED_BYTE, NULL);

        

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    }
 
    void LoadTexture450(TextureOpenGL* texture_implementation, int texture_option_filter, int texture_option_wrap)
    {
        auto& id = texture_implementation->id_;
        auto tex = texture_implementation->Tex();
        auto image = tex->GetImage();

        glCreateTextures(GL_TEXTURE_2D, 1, &id);
    	glTextureStorage2D(id, 1, GL_RGB8, image->width_, image->height_);
        glTextureSubImage2D(id,
            0,
            0, 0,
            image->width_, image->height_,
            GL_RGB, GL_UNSIGNED_BYTE, image->data_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_option_wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_option_wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_option_filter);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_option_filter);
        //glGenerateMipmap(GL_TEXTURE_2D);
    }

    void RenderToTexture450(GLuint& frame_buffer, TextureOpenGL* texture_implementation)
    {
        auto& id = texture_implementation->id_;
        auto& depths_id_ = texture_implementation->depths_id_;
        auto tex = texture_implementation->Tex();
        auto image = tex->GetImage();
    	
        if (frame_buffer) {
            glDeleteFramebuffers(1, &frame_buffer);
            glDeleteTextures(1, &id);
            glDeleteTextures(1, &depths_id_);
            id = 0;
            depths_id_ = 0;
        }

        glCreateFramebuffers(1, &frame_buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

        glCreateTextures(GL_TEXTURE_2D, 1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->width_, image->height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);


        glCreateTextures(GL_TEXTURE_2D, 1, &depths_id_);
        glBindTexture(GL_TEXTURE_2D, depths_id_);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, image->width_, image->height_);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depths_id_, 0);

        LOG_DEBUG("Framebuffer Complete? Ouput: {}", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        //LOG_ENGINE("Framebuffer not complete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        LOG_ENGINE("Created Texture {} and Framebuffer {}", id, frame_buffer);
    }

	
}