#pragma once
#include "../core.h"
#include "../render/render_engine.h"
#include "../engine/engine_sidebar.h"
#include "../engine/engine_console.h"
#include "../engine/engine_gui.h"



namespace kipod{
class MenuModule{
    friend class Module;
protected:

    virtual void Draw()=0;
public:
    virtual ~MenuModule() = default;
};

class Module{
protected:
    std::shared_ptr<RenderScene> scene_;
    std::unique_ptr<Sidebar> sidebar_;
    std::unique_ptr<Console> console_;
    std::unique_ptr<MenuModule> menu_;
    std::string name_;
public:
    Module() = default;
    virtual ~Module() = default;
    Module(Module&&) = default;
    Module& operator=(Module&&) = default;
    Module(std::string name) : name_(name){}
    std::string Name();
    void Name(std::string name);

    virtual void Init();
    virtual void DrawScene();
    virtual void DrawSidebar();
    virtual void DrawConsole();
    virtual void Signup();
    virtual void RemoveSubscription();
    virtual void DrawMenu();
};

}

