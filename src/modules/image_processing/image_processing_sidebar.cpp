#include "image_processing_sidebar.h"
#include "image_processing_scene.h"
#include "../../engine/engine_gui.h"

void kipod::ImageProcessing::ImageProcessingSidebar::SideBarContent()
{
    AddImage();
    ImageList();
    Filter();
    FilterList();
}

void kipod::ImageProcessing::ImageProcessingSidebar::AddImage()
{
    auto scene = std::static_pointer_cast<ImageProcessingScene>(scene_);

    static ImGuiFs::Dialog dlg;

    ImGui::PushID("Add Image");
    const bool browseButtonPressed = ImGui::Button("Add##Image");
    ImGui::PopID();
    const char* chosenPath = dlg.chooseFileDialog(browseButtonPressed);
    if (strlen(chosenPath)>0) {
        auto path = std::filesystem::path(chosenPath);
        if(path.extension() != ".jpg") {
            LOG_ENGINE("Not an jpg file.");
            LOG_CONSOLE("Not an jpg file.");
            return;
        }
        LOG_ENGINE("Loaded image from path {}.",chosenPath);
        LOG_CONSOLE("Chosen file: \"%s\"",chosenPath);
        scene->LoadImage(chosenPath);
    }
     ImGui::Separator();
}

void kipod::ImageProcessing::ImageProcessingSidebar::ImageList()
{

}

void kipod::ImageProcessing::ImageProcessingSidebar::Filter()
{
    auto scene = std::static_pointer_cast<ImageProcessingScene>(scene_);
    glm::mat3 kernel = *scene->ActiveKernel();

    ImGui::Text("Active Kernel:");
    static float* f = &kernel[0][0];
    for (int i = 0; i < 3; i++){
            for(int j =0; j <3; j++){
                ImGui::Text("%.1f", *(f+(3*i+j)));
                if(j<2) ImGui::SameLine();
            }
        }
    if(ImGui::SliderFloat("Parameter", &scene->ActiveKernel()->parameter_,-5.0f,5.0f, "%.1f"))
    {
        scene->ActiveKernel()->Update();
        LOG_INFO("New Parameter: {}. "
                 "New Filter:\n {}",
                 scene->ActiveKernel()->parameter_,
                 *scene->ActiveKernel());
    }
    ImGui::Separator();
}

void kipod::ImageProcessing::ImageProcessingSidebar::FilterList()
{
    auto scene = std::static_pointer_cast<ImageProcessingScene>(scene_);
    auto& kernels = scene->kernels_;
    static int selected_filter = 0;
    for (int n = 0; n <  (int)kernels.size(); n++)
    {
        char buf[32];
        strcpy(buf, kernels[n]->name_.c_str());
        if (ImGui::Selectable(buf, selected_filter == n)){
            selected_filter = n;
            scene->active_kernel_ = kernels[n].get();
            }
    }
}