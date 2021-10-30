#pragma once
#include <utility>

#include "../core.h"
#include "../render/render_scene.h"
namespace kipod{

class Sidebar
{
    friend class Module;

protected:
    int width_{};
    std::shared_ptr<RenderScene> scene_;
    explicit Sidebar(std::shared_ptr<RenderScene> scene) : scene_(std::move(scene)) {}

    virtual void Draw();
    virtual void SideBarContent() = 0;
public:
    virtual ~Sidebar() = default;
};

}
