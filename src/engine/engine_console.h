#pragma once
#include "../core.h"
#include "../utils/imgui_console.h"

namespace kipod{

class RenderScene;

class Console
{
    friend class Module;
protected:
    std::shared_ptr<RenderScene> scene_;
    Console(std::shared_ptr<RenderScene> scene) : scene_(scene){}
    virtual void Draw() {}
    virtual void ConsoleContent() {}
public:
    Console() = default;
    virtual ~Console() = default;
    static AppConsole& GetConsole();
    static void DrawAppConsole();
};

}

#define LOG_CONSOLE(...) kipod::Console::GetConsole().AddLog(__VA_ARGS__)
