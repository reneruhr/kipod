#pragma once
#include "../core.h"
#include "../render/render_scene.h"
namespace kipod{

class Sidebar
{
protected:
    int width_;
    std::shared_ptr<RenderScene> scene_;
public:
    Sidebar(std::shared_ptr<RenderScene> scene) : scene_(scene) {}
    virtual void Draw();
    virtual void SideBarContent() = 0;
};

}
