#pragma once
#include "../../kipod.h"

class MeshmodelSidebar : public kipod::Sidebar
{
public:
    MeshmodelSidebar(std::shared_ptr<kipod::RenderScene> scene) : kipod::Sidebar(scene) {}

    virtual void SideBarContent() override;


    void Help();
    void ModelControl();
    void CameraControl();


    void LoadPrimitive();
    void LoadOBJfile();
    void ModelList();
    void ModelViewOptions();
    void ModelMoveOptions();
    void ModelScaleOptions();
    void ModelMaterialOptions();

    void CameraList();
    void CameraViewOption();

};

