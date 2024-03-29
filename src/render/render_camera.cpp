#include "render_camera.h"
namespace kipod
{

float radians(float degree){
    return degree/ M_PI / 2.0f;
}

RenderCamera::RenderCamera(const float fovy, const float aspect, const float zNear, const float zFar) :
    projection_type_(PROJECTIVE), fovy_(fovy), aspect_(aspect), near_(zNear), far_(zFar)
{
    projection_matrix_ = glm::perspective(radians(fovy), aspect, zNear, zFar );
    LookAt({0,0,3}, {0,0,0}, {0,1,0});
}

RenderCamera::RenderCamera(glm::vec3 eye, glm::vec3 at, glm::vec3 up, ProjectionType projection_type)
    : projection_type_(projection_type), eye_(eye), at_(at), up_(up)
{
    assert(length(eye_-at_)>0.0001);
    MakeProjection(projection_type);
    view_matrix_ = glm::lookAt(eye, at, up);
    UpdateProjectionView();
    UpdateInternalCoordinatesAfterTranslation();
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
   UpdateProjectionView();
   UpdateInternalCoordinatesAfterTranslation();
}

void RenderCamera::Move(RenderCamera::Movement direction, float delta_time)
{
    float step_size = velocity_ * delta_time;
     if (direction == FORWARD){
         eye_ += frontCam_ * step_size;
         at_ += frontCam_ * step_size;
     }
     else if (direction == BACKWARD){
         eye_ -= frontCam_ * step_size;
         at_ -= frontCam_ * step_size;
     }
     else if (direction == LEFT){
         eye_ -= rightCam_ * step_size;
         at_ -= rightCam_ * step_size;
     }
     else if (direction == RIGHT){
         eye_ += rightCam_ * step_size;
         at_ += rightCam_ * step_size;

     }
     else if (direction == DOWN){
        eye_ -= upCam_ * step_size;
        at_ -= upCam_ * step_size;
     }
     else if (direction == UP){
        eye_ += upCam_ * step_size;
        at_ += upCam_ * step_size;
     }

     LookAt(eye_, at_, up_);
}

void RenderCamera::Move(const glm::vec3 &translate)
{
    eye_+=translate;
    view_matrix_ = glm::lookAt(eye_, at_, up_);
    UpdateProjectionView();
    LOG_ENGINE("Moved Camera by {},{},{}", translate.x,translate.y,translate.z);
}

void RenderCamera::Rotate(float x_offset, float y_offset_)
{
    x_offset *= sensitivity_;
    y_offset_ *= sensitivity_;

    yaw_   += x_offset;
    pitch_ += y_offset_;


        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;

    UpdateInternalCoordinatesAfterRotation();
}

void RenderCamera::MakeProjection(RenderCamera::ProjectionType type)
{
    if(type == ORTHOGONAL) projection_matrix_ = glm::ortho(left_, right_, bottom_, top_, near_, far_);
    else projection_matrix_ = glm::perspective(radians(fovy_), aspect_, near_, far_ );
}

void RenderCamera::UpdateInternalCoordinatesAfterTranslation()
{
    frontCam_ = -eye_+at_;
    rightCam_ = glm::normalize(glm::cross(frontCam_, up_));
    upCam_    = glm::normalize(glm::cross(rightCam_, frontCam_));
}

void RenderCamera::UpdateInternalCoordinatesAfterRotation()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    frontCam_ = glm::normalize(front);
    rightCam_ = glm::normalize(glm::cross(frontCam_, up_));
    upCam_    = glm::normalize(glm::cross(rightCam_, frontCam_));
    at_ = eye_ + glm::length(at_-eye_)*frontCam_;
    LookAt(eye_, at_, up_);
}

void RenderCamera::UpdateAt(glm::vec3 at)
{
    at_ = at;
    LookAt(eye_, at_, up_);
}

void RenderCamera::ChangePerspective(const float fovy, const float aspect, const float zNear, const float zFar)
{
    projection_matrix_ = glm::perspective(radians(fovy), aspect, zNear, zFar );
    UpdateProjectionView();
    fovy_ = fovy;
    aspect_ = aspect;
    near_= zNear;
    far_= zFar;
}

void RenderCamera::ScaleOrthogonalCamera(float s)
{
    bottom_-=s;
    top_+=s;
    left_-=s;
    right_+=s; 
    projection_matrix_ = glm::ortho(left_, right_, bottom_, top_, near_, far_);
    UpdateProjectionView();
}

Screen RenderCamera::GetFrontScreen()
{
    Screen screen;
    auto inverse_view = glm::inverse(projection_view_matrix_);
    auto bl = inverse_view*glm::vec4(-1,-1, 1, 1);
    auto br = inverse_view*glm::vec4( 1,-1, 1, 1);
    auto tr = inverse_view*glm::vec4( 1, 1, 1, 1);
    auto tl = inverse_view*glm::vec4(-1, 1, 1, 1);

    screen.left_bottom_ = bl/bl.w;
    screen.right_bottom_ = br/br.w;
    screen.right_top_ = tr/tr.w;
    screen.left_top_ = tl/tl.w;

    return screen;

}


} // kipod
