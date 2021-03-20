#pragma once
#include "../render_camera.h"

namespace kipod{

class SoftRendererUniform
{
public:
    RenderCamera* camera_;
    mat4 object_transform_;
    SoftRendererUniform();
};

}
