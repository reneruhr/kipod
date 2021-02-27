#pragma once

#include "../core.h"
#include "engine_clock.h"
#include "engine_controls.h"
#include "engine_events.h"
#include "engine_gui.h"
#include "engine_menu.h"
#include "engine_input.h"
#include "engine_transform.h"
#include "engine_module.h"
#include "engine_sidebar.h"
#include "engine_console.h"

#include "../render/render_engine.h"

namespace kipod{

class Application
{
    std::unordered_map< std::string, std::unique_ptr<Module> > modules_;
    std::shared_ptr<Window> window_;
    std::unique_ptr<Clock> clock_;
    std::unique_ptr<Menu> menu_;
    std::unique_ptr<Console> console_ = std::make_unique<Console>();

    std::string active_module_ = "";

    int width_, height_;
    int sidebar_width_ = 200;
    int console_height_ = 200;


public:
    Application(int width = 1024, int height = 768);

    void Init();
    void Run();
    void ShutDown();

    void Add(Module&& module);
    void Add(std::string name, kipod::Module &&module);

    void ActiveModule(std::string name);
    Module& ActiveModule();
};

}
