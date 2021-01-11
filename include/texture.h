#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"
#include "utils/image_loader.h"
#include "utils/log.h"

class Texture
{
    unsigned int id_;
    Image* image_;

public:
    Texture();

    void LoadTexture(const char path[]){

           image_= LoadImage(path);


           glGenTextures(1, &id_);
           glBindTexture(GL_TEXTURE_2D, id_); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

           // set the texture wrapping parameters
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

           // set texture filtering parameters
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

           if (image_->data_)
           {
               glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_->width_, image_->height_, 0, GL_RGB, GL_UNSIGNED_BYTE, image_->data_);
               glGenerateMipmap(GL_TEXTURE_2D);
           }
           else
           {
               LOG("Failed to load texture");
           }

           FreeImage(image_);
    }

    void BindTexture(){
        glBindTexture(GL_TEXTURE_2D, id_);
    }
};

#endif // TEXTURE_H
