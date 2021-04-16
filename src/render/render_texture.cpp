#include "render_texture.h"
#include "opengl/opengl_layout.h"
#include "opengl/opengl_engine.h"

void kipod::Texture::LoadTexture(std::filesystem::path path, int texture_option_filter, int texture_option_wrap){

       image_= std::make_unique<Image>(ImageLoader::Kipod_LoadImage(path));

       glGenTextures(1, &id_);
       glBindTexture(GL_TEXTURE_2D, id_);

       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_option_wrap);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_option_wrap);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_option_filter);
       //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_option_filter);

       if (image_->data_)
       {
           glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_->width_, image_->height_, 0, GL_RGB, GL_UNSIGNED_BYTE, image_->data_);
           //glGenerateMipmap(GL_TEXTURE_2D);
       }
       else
       {
           LOG("Failed to load texture");
       }

       ImageLoader::FreeImage(image_.get());
}


void kipod::Texture::RenderToTexture(GLuint& frame_buffer)
{
        if(frame_buffer){
            glDeleteFramebuffers(1, &frame_buffer);
            glDeleteTextures(1, &id_);
            glDeleteTextures(1, &depths_id_);
            id_=0;
            depths_id_=0;
        }

        glGenFramebuffers(1, &frame_buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_->width_, image_->height_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glGenRenderbuffers(1, &depths_id_);
        glBindRenderbuffer(GL_RENDERBUFFER, depths_id_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, image_->width_, image_->height_);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depths_id_);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, id_, 0);

        GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, draw_buffers);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_ENGINE("Failed to Render to Texture");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

kipod::Texture::Texture(int w, int h){
    image_ = std::make_unique<Image>(Image(w,h));
}


kipod::Texture::~Texture(){
    glDeleteTextures(1, &depths_id_);
    glDeleteTextures(1, &id_);
}

void kipod::Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, id_);
}

// From Hazel:
void kipod::Texture::RenderToTexture2(GLuint& frame_buffer)
{
        if(frame_buffer){
            glDeleteFramebuffers(1, &frame_buffer);
            glDeleteTextures(1, &id_);
            glDeleteTextures(1, &depths_id_);
            id_=0;
            depths_id_=0;
        }

            if(OpenGLEngine::Version()==450)           glCreateFramebuffers(1, &frame_buffer);
            else    glGenFramebuffers(1, &frame_buffer);



            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

            if(OpenGLEngine::Version()==450)           glCreateTextures(GL_TEXTURE_2D, 1, &id_);
            else    glGenTextures(1, &id_);
            glBindTexture(GL_TEXTURE_2D, id_);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image_->width_, image_->height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id_, 0);


            if(OpenGLEngine::Version()==450)           glCreateTextures(GL_TEXTURE_2D, 1, &depths_id_);
            else    glGenTextures(1, &depths_id_);
            glBindTexture(GL_TEXTURE_2D, depths_id_);
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, image_->width_, image_->height_);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depths_id_, 0);


           assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
                //LOG_ENGINE("Framebuffer not complete!");

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            LOG_ENGINE("Created Texture {} and Framebuffer {}", id_, frame_buffer);
}

void kipod::Texture::SetupTextureToSquare(float height)
{
    textured_square_ = std::unique_ptr<TexturedShape>(new TexturedShape(Shapes::Square()));
    float ratio = image_->width_/image_->height_;
    textured_square_->ScaleShape(ratio*height, height);
    textured_square_->UpdatedTransformedVertices();
    textured_square_->Init();
    textured_square_->texture_ = this;
    static_cast<GLRenderLayout*>(textured_square_->Layout())->sha_ = std::make_shared<Shader>("passthrough.vert.glsl", "passthrough.frag.glsl");
    static_cast<GLRenderLayout*>(textured_square_->Layout())->sha_->AttachUniform<int>("tex");
}

void kipod::Texture::Draw()
{
    textured_square_->Draw();
}

void kipod::Texture::Resize(int w,int h)
{
    image_->width_ = w;
    image_->height_ = h;
}

void kipod::TexturedShape::Draw()
{
    auto shader = static_cast<GLRenderLayout*>(Layout())->sha_;
    if(shader) shader->Use();
    glActiveTexture(GL_TEXTURE0);
    texture_->Bind();
    if(shader) shader->SetUniform<int>(name_.c_str(), 0);
    Shape::Draw();
    if(shader) shader->Unuse();
}
