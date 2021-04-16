#include "image_processing_module.h"
#include "image_processing_sidebar.h"
#include "image_processing_scene.h"
#include "image_processing_console.h"
namespace kipod::ImageProcessing{

ImageProcessingModule::ImageProcessingModule(std::string name,  int width, int height) : Module(name)
{
    LOG_INFO("ImageProcessing Module constructed");
    scene_ = std::make_shared<ImageProcessingScene>(ImageProcessingScene(width, height));
    sidebar_ = std::make_unique<ImageProcessingSidebar>(ImageProcessingSidebar(scene_));
    console_ = nullptr; // std::make_unique<ImageProcessingSidebar>(ImageProcessingConsole(scene_));;
    menu_= std::make_unique<ImageProcessingModuleMenu>(static_cast<ImageProcessingScene*>(scene_.get()));
}

void ImageProcessingModuleMenu::Draw(){
    if(ImGui::MenuItem("ImageProcessingToggle Option", "", scene_->Toggle("ImageProcessingToggle")))
        scene_->Toggle("ImageProcessingToggle").Switch();
}

}
