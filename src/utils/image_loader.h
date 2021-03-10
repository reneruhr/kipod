#pragma once
#include <type_traits>
#include <filesystem>

struct Image{
    int width_ = 0;
    int height_ = 0;
    int n_channels_ = 0;
    unsigned char* data_ = nullptr;

    Image() = default;
    Image(int w, int h) : width_(w), height_(h){}
    Image(Image&&) = default;
    Image& operator=(Image&&) = default;

};

class ImageLoader{
public:

static Image&& Kipod_LoadImage(std::filesystem::path path);

static void FreeImage(Image* old_image);
};
