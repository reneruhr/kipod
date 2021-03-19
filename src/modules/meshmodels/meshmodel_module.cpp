#include "meshmodel_module.h"
#include "meshmodel_scene.h"
#include "meshmodel_sidebar.h"
#include "meshmodel_console.h"

namespace kipod::MeshModels{
MeshModelModule::MeshModelModule(int width, int height)
{
    LOG_INFO("Meshmodel Module constructed");
    scene_ = std::make_shared<MeshModelScene>(MeshModelScene(width, height));
    sidebar_ = std::make_unique<MeshmodelSidebar>(MeshmodelSidebar(scene_));
    console_ = nullptr;
            //std::make_unique<MeshModelConsole>(MeshModelConsole(scene_));
}
}
