#pragma once

#include "GL/glew.h"
#include "../utils/image_loader.h"
#include "../utils/log.h"

#include "vector"

namespace kipod{

class Texture
{

    Image* image_;


public:
    Texture() = default;
    Texture(int w, int h){
        image_ = new Image(w,h);
    }

    ~Texture(){
        glDeleteTextures(1, &depth_render_buffer);
        glDeleteTextures(1, &id_);
    }


    unsigned int id_;
    std::string name_ ="tex";
    GLuint depth_render_buffer;

    void LoadTexture(const char path[]);

    void RenderToTexture(GLuint& frame_buffer);

    void Bind() const {
        glBindTexture(GL_TEXTURE_2D, id_);
    }
    void RenderToTexture2(GLuint &frame_buffer);
};



class TextureManager{
    inline static std::vector<Texture*> textures_;

public:

//    static void Init(){
//        std::vector<Texture*> textures_ = {};
//    }

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
