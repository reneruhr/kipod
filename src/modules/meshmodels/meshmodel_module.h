#pragma once
#include "kipod.h"
#include "meshmodel_scene.h"
namespace kipod::MeshModels{

class MeshModelModule : public kipod::Module
{
public:
    MeshModelModule(std::string name, int width = 1024, int height = 768);

    auto GetScene() -> MeshModelScene*;
};

class MeshModelModuleMenu : public kipod::MenuModule
{
    friend class MeshModelModule;
    MeshModelScene* scene_;
protected:
    void Draw() override;
public:
    MeshModelModuleMenu(MeshModelScene* scene)
        : scene_(scene){}
};
}
