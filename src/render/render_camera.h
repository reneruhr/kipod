#pragma once
#include "../core.h"

namespace kipod
{
class RenderCamera
{
public:
    int id;
    RenderCamera() = default;
    virtual ~RenderCamera() = default;
    RenderCamera(const float fovy, const float aspect, const float zNear, const float zFar);
    RenderCamera(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);

    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;
    glm::mat4 projection_view_matrix_;

    glm::vec3 eye_, at, up;

    void LookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up );
    void UpdatepojectionView(){ projection_view_matrix_ = projection_matrix_*view_matrix_; }

    operator glm::mat4 () const {   return projection_view_matrix_; }

};

} // kipod
