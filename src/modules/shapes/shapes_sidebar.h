#pragma once
#include "../../kipod.h"


namespace kipod::Shapes{
class ShapesSidebar :
        public kipod::Sidebar{
        friend class ShapesModule;

protected:
    ShapesSidebar(std::shared_ptr<kipod::RenderScene> scene) : kipod::Sidebar(scene) {}

    virtual void SideBarContent() override;

    void AddShape();
    void ShapeList();

};

}
