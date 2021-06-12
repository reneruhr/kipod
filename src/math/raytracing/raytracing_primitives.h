#pragma once
#include <glm/matrix.hpp>
namespace kipod{

enum class RaytracingPrimitive{
    Sphere,
    Ray,
    Quadratic
};

enum class Quadric{
    Unspecified,
    Sphere,
    Ellipsoid,
//    Cylinder,
//    Cone,
//    HyperbolicParaboloid,
//    Paraboloid,
//    HyperboloidOneSheet,
//    HyperboloidTwoSheets
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

struct RaytracingQuadric : public RaytracingObject{
    glm::mat4 Q_ =glm::mat4(1.0);
    glm::mat4 Q_transformed_ = Q_;
    Quadric quadratic_type_;

    RaytracingQuadric();
    RaytracingQuadric(glm::mat4 Q);

    RaytracingQuadric(Quadric type);

    void Transform(glm::mat4 M){
        Q_transformed_ = glm::transpose(M)*Q_*M;
    }

};

}
