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
    friend class Scene;
public:
    PointSet(): Lattice(3), world_transform_(mat4(1.0)){}
    PointSet(mat4 basis): Lattice(basis,3), world_transform_(mat4(1.0)){}
    void Init(GLRenderer *glrenderer);
    virtual void Draw(GLRenderer *glrenderer);
    mat4 GetWorldTransform(){
        return world_transform_;
    }

protected:
    int sample_info_[3] = {10,10,10};
    int sample_size_ = (2*sample_info_[0]+1)*(2*sample_info_[1]+1)*(2*sample_info_[2]+1);
    mat4 world_transform_;
    shared_ptr<LatticeData> lattice_data_= nullptr;



};

#endif // POINTSET_H
