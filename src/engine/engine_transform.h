#pragma once
#include <memory>

#include "../core.h"

namespace kipod{

class Transform
{
    entt::sigh<void(bool)> move_signal_;
    entt::sigh<void(bool)> scale_signal_;
protected:
    std::unique_ptr<glm::mat4> matrix_;
public:
    Transform() { matrix_ = std::make_unique<glm::mat4>(1.0f); }
    explicit Transform(glm::mat4&& matrix){ matrix_ = std::make_unique<glm::mat4>(matrix);    }
    explicit Transform(glm::mat4& matrix){ matrix_ = std::make_unique<glm::mat4>(matrix);    }

    Transform(Transform& other) : matrix_{new glm::mat4(*other.matrix_)} {}
    Transform& operator =(const Transform& other) { matrix_ = std::make_unique<glm::mat4>(*other.matrix_); return *this; }
    Transform(Transform&&) noexcept = default;
    Transform& operator=(Transform&&) noexcept = default;

    operator glm::mat4& ()  { assert(matrix_!=nullptr); return *matrix_; }
    operator const glm::mat4& () const { assert(matrix_!=nullptr); return *matrix_; }

    entt::sink<void(bool)> move_sink_{move_signal_};
    entt::sink<void(bool)> scale_sink_{scale_signal_};

    float& x()  { return (*matrix_)[3][0]; }
    float& y()  { return (*matrix_)[3][1]; }
    float& z()  { return (*matrix_)[3][2]; }

    const glm::mat4& Translate(const glm::vec3& v);
    const glm::mat4& Scale(const glm::vec3& v);
    void  MultiplyFromLeft(const glm::mat4&);
    void  Replace(const glm::mat4&);
};
}
