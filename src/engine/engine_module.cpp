#include "engine_module.h"
#include "engine_menu.h"


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

void kipod::Module::DrawScene()
{
    scene_->Draw();
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
        ImGui::EndMenuBar();
    }
   }
}

