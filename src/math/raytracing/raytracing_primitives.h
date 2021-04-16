#pragma once
#include "../../siegel2021.h"
namespace kipod{

using Vec3d = siegel2021::Vector<double,3>;

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
    double radius_ = 1.;
    Vec3d center_ = {0.,0.,0.};

    RaytracingSphere() : RaytracingObject(RaytracingPrimitive::Sphere){}
};

}
