#pragma once
#include "../../kipod.h"

class MeshmodelSidebar : public kipod::Sidebar
{
    friend class MeshModelModule;

    void Help();
    void ModelControl();
    void CameraControl();
    void LightControl();


    void LoadPrimitive();
    void LoadOBJfile();
    void ModelList();
    void ModelViewOptions();
    void ModelMoveOptions();
    void ModelScaleOptions();
    void ModelMaterialOptions();

    void CameraList();
    void CameraViewOption();

    void LightOptions();
    void LightAdd();
    void LightViewOptions();
protected:
    MeshmodelSidebar(std::shared_ptr<kipod::RenderScene> scene) : kipod::Sidebar(scene) {}

    virtual void SideBarContent() override;

};

