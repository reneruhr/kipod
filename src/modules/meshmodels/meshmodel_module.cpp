#include "meshmodel_module.h"
#include "meshmodel_scene.h"
#include "meshmodel_sidebar.h"
#include "meshmodel_console.h"
#include "../../render/render_engine.h"

namespace kipod::MeshModels{
MeshModelModule::MeshModelModule(std::string name, int width, int height) : Module(name)
{
    LOG_INFO("Meshmodel Module constructed");
    scene_ = std::make_shared<MeshModelScene>(MeshModelScene(width, height));
    sidebar_ = std::make_unique<MeshmodelSidebar>(MeshmodelSidebar(scene_));
    console_ = nullptr;
    menu_= std::make_unique<MeshModelModuleMenu>(static_cast<MeshModelScene*>(scene_.get()));
}

void MeshModelModuleMenu::Draw(){
    if(ImGui::MenuItem("OpenGL" ,  "", RenderEngine::ActiveAPI() == "OpenGL")){
        RenderEngine::SetAPI("OpenGL");
        scene_->NeedsUpdate();
    }
    if(ImGui::MenuItem("SoftRenderer",  "", RenderEngine::ActiveAPI() == "SoftRenderer")){
        RenderEngine::SetAPI("SoftRenderer");
        scene_->NeedsUpdate();
    }
    if(ImGui::MenuItem("Raytracer",  "", RenderEngine::ActiveAPI() == "Raytracer")){
        RenderEngine::SetAPI("Raytracer");
        scene_->NeedsUpdate();
    }
    if(ImGui::MenuItem("Lazy Updates", "only updates on change", scene_->Toggle("Lazy Mode")))
        scene_->Toggle("Lazy Mode").Switch();
}


}
