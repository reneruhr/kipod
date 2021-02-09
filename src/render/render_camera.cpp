#include "render_camera.h"
namespace kipod
{

RenderCamera::RenderCamera(const float fovy, const float aspect, const float zNear, const float zFar)
{
    projection_matrix_ = glm::perspective(fovy, aspect, zNear, zFar );

}
RenderCamera::RenderCamera( const float left, const float right,
                            const float bottom, const float top,
                            const float zNear, const float zFar ){
    projection_matrix_ = glm::ortho(left, right, bottom, top, zNear, zFar);

}

void RenderCamera::LookAt(const glm::vec3 &eye, const glm::vec3 &at, const glm::vec3 &up)
{
   view_matrix_ = glm::lookAt(eye, at, up);
   UpdatepojectionView();
}


} // kipod
