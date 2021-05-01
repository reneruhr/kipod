#include "image_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../external_libs/stb/stb_image.h"


ImageData&& ImageLoader::Kipod_LoadImage(std::filesystem::path path)
{
    ImageData* new_image = new ImageData();
        stbi_set_flip_vertically_on_load(1);
        new_image->data_ = stbi_load(path.string().c_str(), &new_image->width_,
                              &new_image->height_, &new_image->n_channels_, 3);
        return std::move(*new_image);

}

void ImageLoader::FreeImage(ImageData*old_image)
{
        stbi_image_free(old_image->data_);
}
