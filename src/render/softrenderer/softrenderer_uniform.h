#pragma once
#include "../render_camera.h"

namespace kipod{

struct SoftRendererUniform
{
public:
    RenderCamera* camera_;
    mat4 object_transform_;
};

}
