#pragma once
#include "../../kipod.h"

namespace kipod::MeshModels{

class MeshModelModule : public kipod::Module
{
public:
    MeshModelModule(std::string name, int width = 1024, int height = 768);
};


class MeshModelModuleMenu : public kipod::MenuModule
{
    friend class MeshModelModule;
protected:
    void Draw() override;
public:
};


}
