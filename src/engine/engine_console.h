#pragma once
#include "../core.h"
#include "../render/render_scene.h"
#include "../utils/imgui_console.h"

namespace kipod{

class Console
{
protected:
    std::shared_ptr<RenderScene> scene_;


public:
    Console(std::shared_ptr<RenderScene> scene) : scene_(scene){
    }
    virtual void Draw();
    virtual void ConsoleContent() {}
    static ExampleAppConsole& GetConsole();
};

}

#define LOG_CONSOLE(...) kipod::Console::GetConsole().AddLog(__VA_ARGS__)
