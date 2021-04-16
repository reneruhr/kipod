#pragma once
#include "../../kipod.h"
namespace kipod::ImageProcessing{


class ImageProcessingScene;
class Image : public kipod::RenderObject
{
protected :
    //std::unique_ptr<ImageProcessingData> imageprocessingdata_;
    std::unique_ptr<TexturedShape> textured_rectangle_;
public:
    Image() = default;
    Image(std::filesystem::path path);
    virtual ~Image(void) = default;
    Image(Image&&) = default;
    Image& operator=(Image&&) = default;

    void Init();
    void Draw() override;
    int Height();
    int Width();
};
}
