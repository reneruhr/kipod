#pragma once
#include "image_loader.h"

namespace kipod{
class ImageWriter{
public:
    static void WriteImage(const ImageData& image, std::string name);
};

}
