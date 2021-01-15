#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"
#include "utils/image_loader.h"
#include "utils/log.h"

#include "vector"


class Texture
{

    Image* image_;


public:
    Texture() = default;
    Texture(int w, int h){
        image_ = new Image(w,h);
    }

    unsigned int id_;
    std::string name_ ="tex";

    void LoadTexture(const char path[]);

    void RenderToTexture(GLuint& frame_buffer);

    void BindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, id_);
    }
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


#endif // TEXTURE_H
