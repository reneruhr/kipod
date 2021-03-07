#include "template_module.h"
#include "template_sidebar.h"
#include "template_scene.h"
#include "template_console.h"
namespace kipod::Template{

TemplateModule::TemplateModule(int width, int height)
{
    LOG_INFO("Template Module constructed");
    scene_ = std::make_shared<TemplateScene>(TemplateScene(width, height));
    sidebar_ = std::make_unique<TemplateSidebar>(TemplateSidebar(scene_));
    console_ = std::make_unique<TemplateSidebar>(TemplateConsole(scene_));;
}
}
