#include "engine_transform.h"



const glm::mat4 &kipod::Transform::Translate(glm::vec3 translate)
{
    *matrix_ = glm::translate(*matrix_, translate);
    LOG_ENGINE("Moved Model by {},{},{}", translate.x,translate.y,translate.z);
    return *matrix_;
}

const glm::mat4 &kipod::Transform::Scale(glm::vec3 v)
{
    *matrix_ = glm::scale(*matrix_, v);
    return *matrix_;
}
