#pragma once
#include "../core.h"

namespace kipod{

class Transform
{
protected:
    std::unique_ptr<glm::mat4> matrix_;
public:
    Transform() { matrix_ = std::make_unique<glm::mat4>(1.0f); }
    Transform(glm::mat4&& matrix){ matrix_ = std::make_unique<glm::mat4>(std::move(matrix));    }
    Transform(glm::mat4& matrix){ matrix_ = std::make_unique<glm::mat4>(matrix);    }

    Transform(Transform& other) : matrix_{new glm::mat4(*other.matrix_)} {}
    Transform& operator =(const Transform& other) { matrix_.reset(new glm::mat4(*other.matrix_)); return *this; }
    Transform(Transform&&) noexcept = default;
    Transform& operator=(Transform&&) noexcept = default;

    operator glm::mat4& ()  { assert(matrix_!=nullptr); return *matrix_; }
    operator const glm::mat4& () const { assert(matrix_!=nullptr); return *matrix_; }

    float& x()  { return (*matrix_)[3][0]; }
    float& y()  { return (*matrix_)[3][1]; }
    float& z()  { return (*matrix_)[3][2]; }

    const glm::mat4& Translate(const glm::vec3& v);
    const glm::mat4& Scale(const glm::vec3& v);
    void  MultiplyFromLeft(const glm::mat4&);
    void  Replace(const glm::mat4&);
};
}
