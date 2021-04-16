#pragma once
#include "../../siegel2021.h"
#include "raytracing_primitives.h"
namespace kipod{

using Vec3d = siegel2021::Vector<double,3>;

class Ray : public RaytracingObject
{
    Vec3d origin_;
    Vec3d direction_;
public:
    Ray(const Vec3d& o, const Vec3d& d)
        : RaytracingObject(RaytracingPrimitive::Ray), origin_(o), direction_(d){}
    Vec3d Origin() const { return origin_; }
    Vec3d Direction() const { return direction_; }
    Vec3d At(double t) const { return origin_+t*direction_; }
};

}

