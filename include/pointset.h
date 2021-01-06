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
    PointSet(std::vector< int > sample_size = {-10,10,-10,10,-10,10,-10,10}, mat4 basis = mat4()) : Lattice(sample_size, basis)
    {
        LOG("Call PointSet Constructor with sample_size data {}, {} ,{}, {} ,{} ,{} ,{}, {}",
            sample_size_[0],sample_size_[1],sample_size_[2],sample_size_[3],sample_size_[4],sample_size_[5],
                sample_size_[6],sample_size_[7]);
        LOG("...and basis {}", basis_);
    }
    void Init(GLRenderer *glrenderer, WindowType type = WindowType::Box);
    virtual void Draw(GLRenderer *glrenderer);
    mat4 GetWorldTransform(){
        return world_transform_;
    }
protected:
    mat4 world_transform_;
    shared_ptr<LatticeData> lattice_data_= nullptr;
};

#endif // POINTSET_H
