#pragma once
#include <glm/matrix.hpp>
#include "raytracing_primitives.h"
#include <iostream>

namespace kipod{

class Ray : public RaytracingObject
{
    glm::vec3 origin_;
    glm::vec3 direction_;
public:
    Ray(const glm::vec3& origin, const glm::vec3& direction)
        : RaytracingObject(RaytracingPrimitive::Ray), origin_(origin), direction_(direction){}
    glm::vec3 Origin() const { return origin_; }
    glm::vec3 Direction() const { return direction_; }
    glm::vec3 At(float t) const { return origin_+t*direction_; }

};

std::ostream& operator<<(std::ostream& os,  Ray& ray);

}

