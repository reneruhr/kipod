#pragma once
#include "../../kipod.h"

namespace kipod::ImageProcessing{

class ImageProcessingModule : public kipod::Module
{
public:
    ImageProcessingModule(std::string name, int width = 1024, int height = 768);
};


class ImageProcessingScene;
class ImageProcessingModuleMenu : public kipod::MenuModule
{
    friend class ImageProcessingModule;
    ImageProcessingScene* scene_;
protected:
    void Draw() override;
public:
    ImageProcessingModuleMenu(ImageProcessingScene* scene)
        : scene_(scene){}
};


}

