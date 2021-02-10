#pragma once

#include "../core.h"
#include "engine_clock.h"
#include "engine_controls.h"
#include "engine_events.h"
#include "engine_gui.h"
#include "engine_input.h"
#include "engine_transform.h"

#include "../render/render_engine.h"

namespace kipod{

class Application
{
    std::unordered_map< std::string, std::unique_ptr<RenderScene> > modules_;

public:
    Application();

    void Init(int argc, char **argv);
    void Run();
};

}
