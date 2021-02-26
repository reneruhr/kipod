#pragma once
#include "../../kipod.h"

namespace kipod::MeshModels{

class MeshModelConsole : public kipod::Console
{
friend class MeshModelModule;
protected:
    MeshModelConsole(std::shared_ptr<kipod::RenderScene> scene) : kipod::Console(scene) {}
};

}
