#include "engine_module.h"
#include "engine_menu.h"


namespace kipod{

void Module::SynchronizeLinks(){
    if(menu_)
        menu_->module_ = this;
}

std::string kipod::Module::Name()
{
    return name_;
}

void kipod::Module::Name(std::string name)
{
    name_=name;
}

void kipod::Module::Init()
{
    scene_->Setup();
}

void kipod::Module::UpdateScene()
{
    if(!pause_update_)  scene_->Update();
}

void kipod::Module::DrawScene()
{
    if(!pause_)  scene_->Draw();
    Gui::CreateSceneWindow(scene_.get());
}

void kipod::Module::DrawSidebar()
{
    sidebar_->Draw();
}

void kipod::Module::DrawConsole()
{
    if(console_)
        console_->Draw();
}

void kipod::Module::Signup()
{
    dynamic_cast<Listener*>(scene_.get())->Signup();
}

void kipod::Module::RemoveSubscription()
{
    dynamic_cast<Listener*>(scene_.get())->Remove();
}

void kipod::Module::DrawMenu()
{
   if(menu_){
    if (ImGui::BeginMenuBar())
        {
        if (ImGui::BeginMenu(Name().c_str()))
        {
            menu_->Draw();           
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Controls"))
        {
            if(!pause_){
                if(ImGui::MenuItem("Pause Render", "Stops Draw call"))
                    pause_ = !pause_;
            }
            else{
                if(ImGui::MenuItem("Continue Render", "Runs Draw calls"))
                    pause_ = !pause_;
                if(ImGui::MenuItem("Step Forward", "Runs Draw call once"))
                    scene_->Draw();
            }

            if(!pause_update_){
                if(ImGui::MenuItem("Pause Update", "Stops Update()"))
                    pause_update_ = !pause_update_;
            }
            else{
                if(ImGui::MenuItem("Continue Update", "Runs Update()"))
                    pause_update_ = !pause_update_;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
   }
}

}
