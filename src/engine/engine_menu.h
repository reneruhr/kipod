#pragma once
#include "../core.h"
#include "engine_module.h"
namespace kipod{

class Menu{
public:
    void DrawFiles();
    void DrawModuleMenu(std::unordered_map< std::string, std::unique_ptr<Module> >& modules,
                        std::string& active_module);
};

}



