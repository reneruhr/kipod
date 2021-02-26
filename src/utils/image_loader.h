#pragma once

struct Image{
    int width_;
    int height_;
    int n_channels_;
    unsigned char* data_;

    Image() = default;
    Image(int w, int h) : width_(w), height_(h){}
    Image(Image&&) = default;
    Image& operator=(Image&&) = default;

};

class ImageLoader{
public:

static Image&& LoadImage(const char path[]);

static void FreeImage(Image* old_image);
};
