#include "engine_application.h"

kipod::Application::Application(int width, int height) : width_(width), height_(height)
{

}

void kipod::Application::Init()
{
    Log::Init();
    LOG_ENGINE("Logger Started.");

    window_ = std::shared_ptr<kipod::Window>(new kipod::Window(width_+sidebar_width_, height_+console_height_, "kipod(renderer)"));
    window_->init();
    kipod::Gui::Init(window_);

    clock_ = std::make_unique<Clock>(Clock());
    menu_ = std::make_unique<Menu>(Menu(this));

}

void kipod::Application::Run()
{
    while (!window_->windowShouldClose())
    {
        kipod::Events::Process();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glViewport(0, 0, window_->Width(),window_->Height());
        kipod::Gui::Begin();
        menu_->DrawFiles();
        if(HasActiveModule()) {
            menu_->DrawModuleMenu(modules_, active_module_);
            ActiveModule().DrawMenu();
        }
        menu_->DrawTools();
        Console::DrawAppConsole();
        if(HasActiveModule()) {
            ActiveModule().DrawSidebar();
            ActiveModule().DrawConsole();
            ActiveModule().UpdateScene();
            ActiveModule().DrawScene();
        }
        kipod::Gui::End();
        window_->updateWindow();
        clock_->Synchronize();
    }

}

void kipod::Application::ShutDown()
{
     glfwTerminate();
}

void kipod::Application::Add(kipod::Module&& module)
{
   module.Init();
   auto name = module.Name();
   modules_.insert( {name, std::make_unique<Module>(std::move(module))} );
   ActiveModule(name);
}

void kipod::Application::Add(std::string name, kipod::Module&& module)
{
    module.Init();
    auto  ptr = std::make_unique<Module>(std::move(module));
    auto pair = std::make_pair(name, std::move(ptr));
    modules_.insert(std::move(pair));
    modules_[name]->Name(name);
    ActiveModule(name);
}

kipod::Module& kipod::Application::ActiveModule()
{
    return *modules_[active_module_];
}

void kipod::Application::ActiveModule(std::string name)
{
    assert(modules_.find(name)!= end(modules_));    
    if(HasActiveModule()) modules_[active_module_]->RemoveSubscription();
    active_module_ = name;
    modules_[name]->Signup();
    modules_[name]->SynchronizeLinks();
}

auto kipod::Application::GetModule(std::string name) -> Module*
{
    assert(modules_.find(name)!= end(modules_));    
    return modules_[name].get();
}

auto kipod::Application::CreateBasicModule(const std::string& name) -> std::unique_ptr<BasicModule>
{
    return std::make_unique<BasicModule>(BasicModule{width_, height_, name});
}

void kipod::Application::Add(std::unique_ptr<Module> module)
{
    module->Init();
    auto name = module->Name();
    modules_.insert( {name, std::move(module)} );
    ActiveModule(name);
}
