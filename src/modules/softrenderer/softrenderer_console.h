#pragma once
#include "../../kipod.h"

namespace kipod::SoftRenderer{
	
class SoftRendererConsole :
        public kipod::Console{
        friend class SoftRendererModule;
public:
    SoftRendererConsole(std::shared_ptr<kipod::RenderScene> scene) : kipod::Console(scene) {}

    virtual void Draw() override {
        DrawModuleConsole();
    }
    virtual void ConsoleContent() override;
};

}
