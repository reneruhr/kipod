#pragma once
#include <utility>

#include "../core.h"
#include "../render/render_engine.h"
#include "../engine/engine_sidebar.h"
#include "../engine/engine_console.h"
#include "../engine/engine_gui.h"

namespace kipod{
class Module;
class MenuModule{
    friend class Module;
protected:
    Module* module_ = nullptr;
    virtual void Draw()=0;
public:
    virtual ~MenuModule() = default;
};

class Module{

    bool pause_ = false;
    bool pause_update_ = true;

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
    explicit Module(std::string name) : name_(std::move(name)){}
    std::string Name();
    void Name(std::string name);
    void Pause() { pause_ = true; }
    void Continue() { pause_ = false; }
    void SynchronizeLinks();
    void PauseUpdate() { pause_update_ = true; }
    void ContinuePauseUpdate() { pause_update_ = false; }

    virtual void Init();
    virtual void DrawScene();
    virtual void DrawSidebar();
    virtual void DrawConsole();
    virtual void Signup();
    virtual void RemoveSubscription();
    virtual void DrawMenu();
    auto GetScene() { return scene_; }
    virtual void UpdateScene();
    void DrawSidebarContent() { sidebar_->SideBarContent(); }
};

}

