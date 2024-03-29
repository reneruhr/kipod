#pragma once

#include "meshmodel.h"
#include "../../math/raytracing/raytracing_primitives.h"


namespace kipod::MeshModels{

enum class Primitive{
    Cube,
    Tetrahedron,
    Sphere,
    QuadricPrimitive
};

class PrimMeshModel: public MeshModel{
    Primitive type_;
    std::shared_ptr<RaytracingQuadric> quadric_ = nullptr;
public:
    explicit PrimMeshModel(Primitive primitive, int n = 0);
    explicit PrimMeshModel(Quadric quadric);
    PrimMeshModel(const PrimMeshModel&) = default;
    PrimMeshModel& operator=(const PrimMeshModel&) = default;
    PrimMeshModel(PrimMeshModel&&) = default;
    PrimMeshModel& operator=(PrimMeshModel&&) = default;
    void LoadPrimitive(Primitive primitive, int n = 0);

    Primitive Type(){ return type_;}
    RaytracingQuadric* GetRaytracingQuadric() { assert(type_==Primitive::QuadricPrimitive); return quadric_.get();}
};

}
