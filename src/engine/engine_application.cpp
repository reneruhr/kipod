#include "engine_application.h"

kipod::Application::Application(int width, int height) : width_(width), height_(height)
{

}

void kipod::Application::Init()
{
    Log::Init();
    LOG_ENGINE("Logger Started.");

    window_ = std::shared_ptr<kipod::Window>(new kipod::Window(width_+sidebar_width_, height_+console_height_, "קיפוד(renderer)"));
    window_->init();
    kipod::Gui::Init(window_);

    clock_ = std::make_unique<Clock>(Clock());
    menu_ = std::make_unique<Menu>(Menu());

}

void kipod::Application::Run()
{
    while (!window_->windowShouldClose())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glViewport(0, 0, window_->Width(),window_->Height());
        kipod::Gui::Begin();
        menu_->DrawFiles();
        menu_->DrawModuleMenu(modules_, active_module_);
        ActiveModule().DrawSidebar();
        ActiveModule().DrawConsole();
        ActiveModule().DrawScene();
        kipod::Gui::End();

        window_->updateWindow();
        kipod::Events::Process();
        clock_->Synchronize();
    }
}

void kipod::Application::Add(kipod::Module&& module)
{
   active_module_ = module.Name();
   module.Init();
   modules_.insert( {module.Name(), std::make_unique<Module>(std::move(module))} );
}
void kipod::Application::Add(std::string name, kipod::Module&& module)
{
    module.Init();
    auto  ptr = std::make_unique<Module>(std::move(module));
    auto pair = std::make_pair(name, std::move(ptr));
    modules_.insert(std::move(pair));
    modules_[name]->Name(name);
    active_module_ = name;
}

kipod::Module& kipod::Application::ActiveModule()
{
    return *modules_[active_module_];
}

void kipod::Application::ActiveModule(std::string name)
{
    assert(modules_.find(name)!= end(modules_));
    active_module_ = name;
}


