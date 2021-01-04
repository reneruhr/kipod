#ifndef POINTSET_H
#define POINTSET_H

#include "utils/vec.h"
#include "utils/mat.h"
#include "math/lattice.h"
#include "glrenderer.h"


class PointSet : public Lattice
{
    friend class GLRenderer;
    friend class GUI;
public:
    PointSet(): Lattice(3), _world_transform(mat4(1.0)){}
    PointSet(mat4& basis): Lattice(basis,3), _world_transform(mat4(1.0)){}
    void init(GLRenderer *glrenderer);
    void draw(GLRenderer *glrenderer);
    mat4 getmTransform(){
        return _world_transform;
    }

protected:
    int sampleInfo[3] = {10,10,10};
    int sampleSize = (2*sampleInfo[0]+1)*(2*sampleInfo[1]+1)*(2*sampleInfo[2]+1);
    mat4 _world_transform;
    shared_ptr<LatticeData> latticeData= nullptr;



};

#endif // POINTSET_H
