#include "raytracing_primitives.h"
namespace kipod{

RaytracingQuadric::RaytracingQuadric() : RaytracingObject(RaytracingPrimitive::Quadratic), quadratic_type_(Quadric::Unspecified) {}

RaytracingQuadric::RaytracingQuadric(glm::mat4 Q) : RaytracingObject(RaytracingPrimitive::Quadratic), Q_(Q), quadratic_type_(Quadric::Unspecified) {}

RaytracingQuadric::RaytracingQuadric(Quadric type) : RaytracingObject(RaytracingPrimitive::Quadratic)
{
    switch(type){
    case Quadric::Unspecified:
    {
        break;
    }
    case Quadric::Sphere:
    {
        Q_[3][3] = -1;
        break;
    }
    case Quadric::Ellipsoid:
    {
        Q_[3][3] = -1;
        break;
    }
    }//Switch

}


}
