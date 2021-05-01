#pragma once
#include "../core.h"
#include "render_object.h"
#include "../modules/shapes/shape.h"

namespace kipod{

class TexturedShape : public Shapes::Shape{
public:
    TexturedShape(Polygon polygon) : Shape(polygon){}
    virtual void Draw() override;
    Texture* texture_ = nullptr;
};

class TextureImplementation{
    friend class Texture;
protected:
    Texture* tex_;
public:
    unsigned int id_ = 0;
    unsigned int depths_id_ = 0;
    std::string name_ = "tex";
    virtual void Bind() const = 0;
    virtual void LoadTexture(int texture_option_filter = GL_LINEAR, int texture_option_wrap = GL_REPEAT) = 0;
    virtual void RenderToTexture(GLuint& frame_buffer) = 0;

    virtual void SetupTextureToSquare(float height = 300) = 0;
    virtual void Draw() = 0;
    virtual void Resize(int w, int h) = 0;

    TextureImplementation() = default;
    TextureImplementation(TextureImplementation&&) = default;
    TextureImplementation& operator=(TextureImplementation&&) = default;
    TextureImplementation(Texture* texture) : tex_(texture) {}
    virtual ~TextureImplementation() = default;
};

class Texture
{
    friend class TextureImplementation;
    friend class TextureOpenGL;

    std::unique_ptr<ImageData> image_;
    std::unique_ptr<TexturedShape> textured_square_;
    std::unique_ptr<TextureImplementation> texture_implementation_;

public:
    Texture() = default;
    Texture(int w, int h);
    Texture(Texture&&) = default;
    Texture& operator=(Texture&&) = default;
    ~Texture() = default;
    unsigned int Id() const { return texture_implementation_->id_; }
    std::string Name() const { return texture_implementation_->name_;  }
    void Resize(int w, int h);
    void Init();
    void Bind() const;
    void LoadTexture(std::filesystem::path path, int texture_option_filter = GL_LINEAR, int texture_option_wrap = GL_REPEAT);
    void RenderToTexture(GLuint& frame_buffer);
    void Draw();
    void SetupTextureToSquare(float height = 300);
    ImageData* GetImage() const { return image_.get(); }
    TexturedShape* Square() const { return textured_square_.get(); }
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
