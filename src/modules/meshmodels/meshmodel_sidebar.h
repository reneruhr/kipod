#pragma once
#include <utility>

#include "../../kipod.h"

namespace kipod::MeshModels{
class MeshmodelSidebar : public kipod::Sidebar
{
    friend class MeshModelModule;

    static void Help();
    void ModelControl();
    void CameraControl();
    void LightControl();

    void LoadPrimitive();
    void LoadQuadricPrimitive();
    void LoadOBJfile();
    void ModelList();
    void ModelViewOptions();
    void ModelMoveOptions();
    void ModelScaleOptions();
    void ModelMaterialOptions();

    void CameraList();
    void CameraViewOption();
    void CameraProjectiveOption();

    void LightOptions();
    void LightAdd();
    void LightViewOptions();
protected:
    explicit MeshmodelSidebar(std::shared_ptr<kipod::RenderScene> scene) : kipod::Sidebar(std::move(scene)) {}
public:
    void SideBarContent() override;
};


}
