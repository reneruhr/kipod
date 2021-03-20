#pragma once
#include "../../kipod.h"

namespace kipod::MeshModels{

class MeshModelModule : public kipod::Module
{
public:
    MeshModelModule(int width = 1024, int height = 768);

    virtual void DrawMenu() override;
};

}
