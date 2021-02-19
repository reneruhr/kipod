#ifndef PRIMMESHMODEL_H
#define PRIMMESHMODEL_H

#include "meshmodel.h"

enum Primitive{
    Cube,
    Tetrahedron,
    Sphere
};

class PrimMeshModel: public MeshModel{

public:
    PrimMeshModel(Primitive primitive, int n =0);
    PrimMeshModel(PrimMeshModel&&) = default;
    PrimMeshModel& operator=(PrimMeshModel&&) = default;
    void LoadPrimitive(Primitive primitive, int n =0);
};


#endif // PRIMMESHMODEL_H
