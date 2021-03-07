#pragma once
#include "../../kipod.h"

namespace kipod::SoftRenderer{

class SoftRendererModule : public kipod::Module
{
public:
    SoftRendererModule(int width = 1024, int height = 768);
};

}
