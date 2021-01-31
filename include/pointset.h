#ifndef POINTSET_H
#define POINTSET_H

#include "utils/vec.h"
#include "utils/mat.h"
#include "math/lattice.h"
#include "glrenderer.h"

#include "render_object.h"

class PointSet : public Lattice, public kipod::RenderObject
{
    friend class GLRenderer;
    friend class GUI;
    friend class Scene;
public:
    PointSet(std::vector< int > sample_size = {-10,10,-10,10,-10,10,-10,10}, mat4 basis = mat4()) : Lattice(sample_size, basis)
    {
        LOG("Call PointSet Constructor with sample_size data {}, {} ,{}, {} ,{} ,{} ,{}, {}",
            sample_size_[0],sample_size_[1],sample_size_[2],sample_size_[3],sample_size_[4],sample_size_[5],
                sample_size_[6],sample_size_[7]);
        LOG("...and basis {}", basis_);
    }

    virtual void Init();
    virtual void Draw();
    mat4 GetWorldTransform(){
        return world_transform_;
    }
    void BaseChange(GLRenderer *glrenderer, mat4 new_basis);
protected:
    mat4 world_transform_;
    shared_ptr<LatticeData> lattice_data_= nullptr;

    void SetPointSize(float size){
        lattice_data_->point_size_ = size;
    }

};

#endif // POINTSET_H
