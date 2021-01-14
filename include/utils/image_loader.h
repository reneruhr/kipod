#pragma once


#include "../../vendor/stb/stb_image.h"

struct Image{
    int width_;
    int height_;
    int n_channels_;
    unsigned char* data_;
};

class ImageLoader{
public:

static Image* LoadImage(const char path[])
{
    Image* new_image = new Image();
    stbi_set_flip_vertically_on_load(1);
    new_image->data_ = stbi_load(path, &new_image->width_,
                          &new_image->height_, &new_image->n_channels_, 3);
    return new_image;
}

static void FreeImage(Image* old_image)
{
    stbi_image_free(old_image->data_);
}

};
