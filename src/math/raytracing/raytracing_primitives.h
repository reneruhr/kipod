#pragma once
#include <glm/matrix.hpp>
namespace kipod{

enum class RaytracingPrimitive{
    Sphere,
    Ray
};


struct RaytracingObject
{
    RaytracingObject() = default;
    RaytracingObject(RaytracingPrimitive type) : type_(type){}
    RaytracingPrimitive type_;
};

struct RaytracingSphere : public RaytracingObject{
    float radius_ = 1.;
    glm::vec3 center_ = {0.,0.,0.};

    RaytracingSphere() : RaytracingObject(RaytracingPrimitive::Sphere){}
};

}
