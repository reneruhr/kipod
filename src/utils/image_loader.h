#pragma once
#include <type_traits>
#include <filesystem>
namespace kipod{
struct ImageData{
    int width_ = 0;
    int height_ = 0;
    int n_channels_ = 0;
    unsigned char* data_ = nullptr;

    ImageData() = default;
    ImageData(int w, int h) : width_(w), height_(h){}
    ImageData(int w, int h, int n, unsigned char* data) : width_(w), height_(h), n_channels_(n), data_(data) {}
    ImageData(ImageData&&) = default;
    ImageData& operator=(ImageData&&) = default;

};

class ImageLoader{
public:

static ImageData&& LoadImage(std::filesystem::path path);

static void FreeImage(ImageData* old_image);
};

}
