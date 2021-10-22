#include "image_writer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../external_libs/stb/stb_image_write.h"

namespace kipod{
void ImageWriter::WriteImage(const ImageData& image, std::string name)
{
    stbi_write_png((name+".png").c_str(), image.width_, image.height_, image.n_channels_, image.data_, image.width_ * image.n_channels_);
}

}
