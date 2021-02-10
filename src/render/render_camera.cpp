#include "render_camera.h"
namespace kipod
{

RenderCamera::RenderCamera(const float fovy, const float aspect, const float zNear, const float zFar)
{
    projection_matrix_ = glm::perspective(fovy, aspect, zNear, zFar );
    LookAt({0,0,3}, {0,0,0}, {0,1,0});
}
RenderCamera::RenderCamera( const float left, const float right,
                            const float bottom, const float top,
                            const float zNear, const float zFar ){
    projection_matrix_ = glm::ortho(left, right, bottom, top, zNear, zFar);
    LookAt({0,0,3}, {0,0,0}, {0,1,0});
}

void RenderCamera::LookAt(const glm::vec3 &eye, const glm::vec3 &at, const glm::vec3 &up)
{
   eye_= eye; at_ = at; up_ = up;
   view_matrix_ = glm::lookAt(eye, at, up);
   UpdatepojectionView();
}

void RenderCamera::Move(const glm::vec3 &translate)
{
    eye_+=translate;
    view_matrix_ = glm::lookAt(eye_, at_, up_);
    UpdatepojectionView();
}


} // kipod
