#ifndef RENDERCAMERA_H
#define RENDERCAMERA_H

#include "render_shader.h"
#include <string>

namespace kipod
{
class RenderCamera
{
public:
    int id;
    RenderCamera() = default;
    virtual ~RenderCamera() = default;
    RenderCamera(const float fovy, const float aspect, const float zNear, const float zFar){}
};



} // kipod
#endif // RENDERCAMERA_H
