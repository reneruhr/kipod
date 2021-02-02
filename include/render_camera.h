#ifndef RENDERCAMERA_H
#define RENDERCAMERA_H

#include "core.h"

namespace kipod
{
class RenderCamera
{
public:
    int id;
    RenderCamera() = default;
    virtual ~RenderCamera() = default;
    RenderCamera(const float fovy, const float aspect, const float zNear, const float zFar){}

    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;

    operator glm::mat4 () const {
        return projection_matrix_*view_matrix_;
    }
};



} // kipod
#endif // RENDERCAMERA_H
