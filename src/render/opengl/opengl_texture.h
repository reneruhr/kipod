#pragma once
#include "../render_texture.h"
namespace kipod{
    class TextureOpenGL : public TextureImplementation {
    	
    public:
        TextureOpenGL() = default;
    	TextureOpenGL(Texture* tex) : TextureImplementation(tex) {}
        ~TextureOpenGL() override;
        TextureOpenGL(TextureOpenGL&&) = default;
        TextureOpenGL& operator=(TextureOpenGL&&) = default;
    	
        virtual void Bind() const override;
        virtual void LoadTexture(int texture_option_filter = GL_LINEAR, int texture_option_wrap = GL_REPEAT) override;
        virtual void RenderToTexture(GLuint& frame_buffer) override;

        virtual void SetupTextureToSquare(float height = 300) override;
        virtual void Draw() override;
        virtual void Resize(int w, int h) override;

        Texture* Tex() const { return tex_; }
    };
}
