#include "image.h"

namespace kipod::ImageProcessing{

Image::Image(std::filesystem::path path)
{
        name_ = path.stem().string();

        tex_ = std::make_shared<kipod::Texture>();

        std::cout << path;
        if(std::filesystem::exists(path)){
            tex_->LoadTexture(path, GL_NEAREST, GL_CLAMP_TO_BORDER);
            return;
         }
        LOG_INFO("Tried but did not succeed to load image.");
        tex_ = nullptr;
}

void Image::Init()
{     
    textured_rectangle_ = std::unique_ptr<TexturedShape>(new TexturedShape(Shapes::Rectangle(2, 2)));
    textured_rectangle_->Init();
    textured_rectangle_->texture_ = tex_.get();


}


void Image::Draw()
{
    glActiveTexture(GL_TEXTURE0);
    textured_rectangle_->texture_->Bind();
    textured_rectangle_->Shape::Draw();
}

int Image::Height()
{
    return tex_->GetImage()->height_;
}

int Image::Width()
{
    return tex_->GetImage()->width_;
}


}
