#include "render_texture.h"
#include "opengl/opengl_layout.h"
#include "opengl/opengl_engine.h"
#include "opengl/opengl_texture.h"

void kipod::Texture::LoadTexture(std::filesystem::path path, int texture_option_filter, int texture_option_wrap)
{
       image_= std::make_unique<ImageData>(ImageLoader::LoadImage(path));
       if (image_->data_) {
           texture_implementation_ = std::make_unique<TextureOpenGL>(this);
           texture_implementation_->LoadTexture(texture_option_filter, texture_option_wrap);
       } else {
           LOG("Failed to load texture");
       }
       ImageLoader::FreeImage(image_.get());
}

void kipod::Texture::RenderToTexture(GLuint& frame_buffer)
{
    texture_implementation_->RenderToTexture(frame_buffer);
}

kipod::Texture::Texture(int w, int h)
{
    image_ = std::make_unique<ImageData>(ImageData(w,h));
}

void kipod::Texture::Init()
{
    texture_implementation_ = std::make_unique<TextureOpenGL>(this);
}

void kipod::Texture::Bind() const 
{
    texture_implementation_->Bind();
}

void kipod::Texture::SetupTextureToSquare(float height)
{
    textured_square_ = std::make_unique<TexturedShape>(Shapes::Square());
    float ratio = image_->width_ / image_->height_;
    textured_square_->ScaleShape(ratio * height, height);
    textured_square_->UpdatedTransformedVertices();
    textured_square_->Init();
    textured_square_->texture_ = this;
    texture_implementation_->SetupTextureToSquare(height);
}

void kipod::Texture::Draw()
{
    textured_square_->Draw();
}

void kipod::Texture::Resize(int w,int h)
{
    image_->width_ = w;
    image_->height_ = h;
    texture_implementation_->Resize(w, h);
}

void kipod::TexturedShape::Draw()
{
    auto shader = dynamic_cast<GLRenderLayout*>(Layout())->sha_;
    if(shader) shader->Use();
    glActiveTexture(GL_TEXTURE0);
    texture_->Bind();
    if(shader) shader->SetUniform<int>(name_, 0);
    Shape::Draw();
    if(shader) shader->Unuse();
}

void kipod::Texture::LoadBufferedTexture(std::filesystem::path path)
{
    image_ = std::make_unique<ImageData>(ImageLoader::LoadImage(path));
    if (image_->data_) {
        texture_implementation_ = std::make_unique<TextureIPOpenGL>(this);
        texture_implementation_->LoadTexture();
    } else {
        LOG("Failed to load texture");
    }
    ImageLoader::FreeImage(image_.get());
}
