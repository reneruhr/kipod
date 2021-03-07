#pragma once
#include "../../kipod.h"

namespace kipod::Template{
class TemplateConsole :
        public kipod::Console{
        friend class TemplateModule;
public:
    TemplateConsole(std::shared_ptr<kipod::RenderScene> scene) : kipod::Console(scene) {}

    virtual void Draw() override {
        DrawModuleConsole();
    }
    virtual void ConsoleContent() override;
};

}
