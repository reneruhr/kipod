#pragma once
#include "../core.h"
#include "engine_module.h"

namespace kipod{

class Application;

class Menu{
    Application* app_;
public:
    Menu(Application* app) : app_(app){}
    void DrawFiles();
    void DrawModuleMenu(std::unordered_map< std::string, std::unique_ptr<Module> >& modules,
                        std::string& active_module);
    void DrawTools();

};
}

