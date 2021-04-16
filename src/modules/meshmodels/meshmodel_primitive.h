#pragma once

#include "meshmodel.h"

namespace kipod::MeshModels{

enum Primitive{
    Cube,
    Tetrahedron,
    Sphere
};

class PrimMeshModel: public MeshModel{
    Primitive type_;
public:
    PrimMeshModel(Primitive primitive, int n =0);
    PrimMeshModel(PrimMeshModel&&) = default;
    PrimMeshModel& operator=(PrimMeshModel&&) = default;
    void LoadPrimitive(Primitive primitive, int n =0);

    Primitive Type(){ return type_;}
};

}
