#pragma once
#include "../../kipod.h"

class MeshmodelSidebar : public kipod::Sidebar
{
public:
    MeshmodelSidebar(std::shared_ptr<kipod::RenderScene> scene) : kipod::Sidebar(scene) {}

    virtual void SideBarContent() override;


    void ModelControl();
    void loadPrimitive();
    void loadOBJfile();
};

