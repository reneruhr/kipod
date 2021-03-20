#include "meshmodel_module.h"
#include "meshmodel_scene.h"
#include "meshmodel_sidebar.h"
#include "meshmodel_console.h"
#include "../../render/render_engine.h"

namespace kipod::MeshModels{
MeshModelModule::MeshModelModule(int width, int height)
{
    LOG_INFO("Meshmodel Module constructed");
    scene_ = std::make_shared<MeshModelScene>(MeshModelScene(width, height));
    sidebar_ = std::make_unique<MeshmodelSidebar>(MeshmodelSidebar(scene_));
    console_ = nullptr;
    //std::make_unique<MeshModelConsole>(MeshModelConsole(scene_));
}

void MeshModelModule::DrawMenu()
{
    if (ImGui::BeginMenu(name_.c_str()))
    {
        if(ImGui::MenuItem("OpenGL" ,  "", RenderEngine::ActiveAPI() == "OpenGL"))
            RenderEngine::SetAPI("OpenGL");
        if(ImGui::MenuItem("SoftRenderer",  "", RenderEngine::ActiveAPI() == "SoftRenderer"))
            RenderEngine::SetAPI("SoftRenderer");
    ImGui::EndMenu();
    }
}
}
