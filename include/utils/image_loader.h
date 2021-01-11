#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stb/stb_image.h"

struct Image{
    int width_;
    int height_;
    int n_channels_;
    unsigned char* data_;
};

Image* LoadImage(const char path[])
{
    Image* new_image = new Image();
    new_image->data_ = stbi_load(path, &new_image->width_,
                          &new_image->height_, &new_image->n_channels_, 0);
    return new_image;
}

void FreeImage(Image* old_image)
{
    stbi_image_free(old_image->data_);
}


#endif // IMAGELOADER_H
