#pragma once
#include "../core.h"

namespace kipod{

class Transform
{
protected:
    std::unique_ptr<glm::mat4> matrix_;

public:
    Transform();
    Transform(glm::mat4&& matrix){ matrix_ = std::make_unique<glm::mat4>(matrix);    }

    virtual operator const glm::mat4& () const { assert(matrix_!=nullptr); return *matrix_; }

    float x()  { return (*matrix_)[3][0]; }
    float y()  { return (*matrix_)[3][1]; }
    float z()  { return (*matrix_)[3][2]; }

    const glm::mat4& Translate(glm::vec3 v) { *matrix_ = glm::translate(*matrix_, v); return *matrix_; }
    const glm::mat4& Scale(glm::vec3 v) { *matrix_ = glm::scale(*matrix_, v); return *matrix_; }


};
}
