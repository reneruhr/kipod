#pragma once
#include "../core.h"
#include "../render/render_scene.h"
namespace kipod{

class Sidebar
{
    friend class Module;

protected:
    int width_;
    std::shared_ptr<RenderScene> scene_;
    Sidebar(std::shared_ptr<RenderScene> scene) : scene_(scene) {}
    virtual void Draw();
    virtual void SideBarContent() = 0;
};

}
