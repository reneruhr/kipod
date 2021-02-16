#pragma once
#include "../core.h"
#include "../render/render_engine.h"
#include "../engine/engine_sidebar.h"
#include "../engine/engine_console.h"
#include "../engine/engine_gui.h"

namespace kipod{

class Module{
protected:
    std::shared_ptr<RenderScene> scene_;
    std::unique_ptr<Sidebar> sidebar_;
    std::unique_ptr<Console> console_;
    std::string name_;
public:
    Module() = default;
    Module(Module&& module) = default;
    Module& operator=(Module&& module) = default;
    virtual ~Module() = default;

    std::string Name();
    void Name(std::string name);

    virtual void Init();
    virtual void DrawScene();
    virtual void DrawSidebar();
    virtual void DrawConsole();
};

}

