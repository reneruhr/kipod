#pragma once
#include "../../kipod.h"


namespace kipod::SoftRenderer{
class SoftRendererSidebar :
        public kipod::Sidebar{
        friend class SoftRendererModule;

protected:
    SoftRendererSidebar(std::shared_ptr<kipod::RenderScene> scene) : kipod::Sidebar(scene) {}

    virtual void SideBarContent() override;

    void AddSoftRenderer();
    void SoftRendererList();

};

}
