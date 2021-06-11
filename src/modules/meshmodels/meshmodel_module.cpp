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
        module_->Continue();
        RenderEngine::SetAPI("OpenGL");
        scene_->NeedsUpdate();
    }
    if(ImGui::MenuItem("SoftRenderer",  "Activates Lazy Updates", RenderEngine::ActiveAPI() == "SoftRenderer")){
        module_->Continue();
        RenderEngine::SetAPI("SoftRenderer");
        scene_->Toggle("Lazy Mode").On();
        scene_->NeedsUpdate();
        LOG_CONSOLE("The software renderer is slow. Turned on 'Lazy Mode'. It will only redraw a frame is something was changed.");
    }
    if(ImGui::MenuItem("Raytracer",  "Pause Draw calls", RenderEngine::ActiveAPI() == "Raytracer")){
        module_->Pause();
        RenderEngine::SetAPI("Raytracer");
        scene_->NeedsUpdate();
        LOG_CONSOLE("The software raytracer is not realtime. Use Control->Step Forward to render a frame.");
        LOG_CONSOLE("This needs to be done after any modification of the screen, in particular when resizing.");
    }
    if(ImGui::MenuItem("Lazy Updates", "Only updates on change", scene_->Toggle("Lazy Mode")))
        scene_->Toggle("Lazy Mode").Switch();
}


}
