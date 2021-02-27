#pragma once
#include "../core.h"
#include "render_object.h"
#include "../modules/shapes/shape.h"

namespace kipod{

class TexturedSquare : public Shapes::Shape{
public:
    TexturedSquare(Polygon polygon) : Shape(polygon){}
    virtual void Draw() override;
    Texture* texture_ = nullptr;
};

class Texture
{
    std::unique_ptr<Image> image_;
    std::unique_ptr<TexturedSquare> textured_square_;

public:
    Texture() = default;
    Texture(int w, int h);
    Texture(Texture&&) = default;
    Texture& operator=(Texture&&) = default;
    ~Texture();

    unsigned int id_ = 0;
    unsigned int depths_id_ = 0;
    std::string name_ ="tex";


    void Bind() const;
    void LoadTexture(const char path[]);
    void RenderToTexture(GLuint& frame_buffer);
    void RenderToTexture2(GLuint &frame_buffer);
    TexturedSquare* Square(){ return textured_square_.get(); }

    void SetupTextureToSquare();
    void Draw();
    void Resize(int w,int h);
};



class TextureManager{
    inline static std::vector<Texture*> textures_;

public:

    static Texture* Get(int id)
    {
        return textures_[id];
    }

    static void Add(Texture* texture)
    {
        textures_.push_back(texture);
    }
};

}
