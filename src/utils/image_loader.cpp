#include "image_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../external_libs/stb/stb_image.h"


Image&& ImageLoader::Kipod_LoadImage(const char path[])
{
        Image* new_image = new Image();
        stbi_set_flip_vertically_on_load(1);
        new_image->data_ = stbi_load(path, &new_image->width_,
                              &new_image->height_, &new_image->n_channels_, 3);
        return std::move(*new_image);

}

void ImageLoader::FreeImage(Image *old_image)
{
        stbi_image_free(old_image->data_);
}
