#pragma once
#include "../core.h"

namespace kipod {

struct Screen{
    glm::vec3 left_bottom_;
    glm::vec3 right_bottom_;
    glm::vec3 right_top_;
    glm::vec3 left_top_;
};

class RenderCamera
{
public:
    enum ProjectionType{
        ORTHOGONAL,
        PROJECTIVE
    };
    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    ProjectionType projection_type_;
    int id;
    RenderCamera() = default;
    virtual ~RenderCamera() = default;
    RenderCamera(glm::vec3 eye, glm::vec3 at = {0.0f,0.0f,0.0f}, glm::vec3 up={0.0f,1.0f,0.0f}, ProjectionType projection_type = PROJECTIVE);
    RenderCamera(const float fovy, const float aspect, const float zNear, const float zFar);
    RenderCamera(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);

    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;
    glm::mat4 projection_view_matrix_;

    glm::vec3 eye_, at_, up_;
    glm::vec3 frontCam_, rightCam_, upCam_;
    float fovy_ = 45.0f;
    float aspect_ = 4.f/3.f;
    float near_=1.f, far_=100.f;
    float left_ = -10.0f, right_ = 10.0f, bottom_ = -10.0f, top_ = 10.0f;


    float yaw_ = -90.0f , pitch_ = 0.0f;

    float velocity_ = 1.0f;
    float sensitivity_ = 0.2f;


    void LookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up );
    void UpdateProjectionView(){ projection_view_matrix_ = projection_matrix_*view_matrix_; }
    void Move(Movement direction, float delta_time);
    void Move(const glm::vec3 &translate);
    void Rotate(float x_offset, float y_offset_);
    void MakeProjection(ProjectionType type);
    void UpdateInternalCoordinatesAfterTranslation();
    void UpdateInternalCoordinatesAfterRotation();

    void UpdateAt(glm::vec3 at);

    void ChangePerspective(const float fovy, const float aspect, const float zNear, const float zFar);

    operator glm::mat4 () const {   return projection_view_matrix_; }
    const glm::mat4& operator()() const {   return projection_view_matrix_; }

    void ScaleOrthogonalCamera(float);
    Screen GetFrontScreen();

    glm::vec3 Eye(){ return eye_; }
};

} // kipod
