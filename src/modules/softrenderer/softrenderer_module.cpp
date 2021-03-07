#include "softrenderer_module.h"
#include "softrenderer_sidebar.h"
#include "softrenderer_scene.h"
#include "softrenderer_console.h"
namespace kipod::SoftRenderer{

SoftRendererModule::SoftRendererModule(int width, int height)
{
    LOG_INFO("SoftRenderer Module constructed");
    scene_ = std::make_shared<SoftRendererScene>(SoftRendererScene(width, height));
    sidebar_ = std::make_unique<SoftRendererSidebar>(SoftRendererSidebar(scene_));
    console_ = nullptr;
}
}
