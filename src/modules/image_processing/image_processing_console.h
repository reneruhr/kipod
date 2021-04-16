#pragma once
#include "../../kipod.h"

namespace kipod::ImageProcessing{
class ImageProcessingConsole :
        public kipod::Console{
        friend class ImageProcessingModule;
public:
    ImageProcessingConsole(std::shared_ptr<kipod::RenderScene> scene) : kipod::Console(scene) {}

    virtual void Draw() override {
        DrawModuleConsole();
    }
    virtual void ConsoleContent() override;
};

}
