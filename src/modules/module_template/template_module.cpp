#include "template_module.h"
#include "template_sidebar.h"
#include "template_scene.h"
#include "template_console.h"
namespace kipod::Template{

TemplateModule::TemplateModule(std::string name,  width, int height) : Module(name)
{
    LOG_INFO("Template Module constructed");
    scene_ = std::make_shared<TemplateScene>(T
    	emplateScene(width, height));
    sidebar_ = std::make_unique<TemplateSidebar>(TemplateSidebar(scene_));
    console_ = nullptr // std::make_unique<TemplateSidebar>(TemplateConsole(scene_));;
    menu_= std::make_unique<TemplateModuleMenu>(static_cast<TemplateScene*>(scene_.get()));
}

void TemplateModuleMenu::Draw(){
    if(ImGui::MenuItem("TemplateToggle Option", "", scene_->Toggle("TemplateToggle")))
        scene_->Toggle("TemplateToggle").Switch();
}


