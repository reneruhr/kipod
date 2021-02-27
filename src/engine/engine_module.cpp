#include "engine_module.h"


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
    else
        Console::DrawAppConsole();
}

void kipod::Module::Signup()
{
    dynamic_cast<Listener*>(scene_.get())->Signup();
}
void kipod::Module::RemoveSubscription()
{
    dynamic_cast<Listener*>(scene_.get())->Remove();
}
