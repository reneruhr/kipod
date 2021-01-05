#ifndef LATTICE_H
#define LATTICE_H

#include "core.h"

class Lattice
{
public:
    Lattice(vec3 basis3d[3]): dim_(3){
        for(int i = 0;i<3;i++) basis_[i]=vec4(basis3d[i],0);
    }
    Lattice(mat3& basis3d): dim_(3){
        for(int i = 0;i<3;i++) basis_[i]=vec4(basis3d[i],0);
    }
    Lattice(mat4& basis4d, int dim=4): dim_(dim),basis_(basis4d){}
    Lattice(int dim=4): dim_(dim), basis_(mat4(1.0)){}

    mat4 GetBasisMatrix(){
        return basis_;
    }

    int dim_;
    mat4 basis_;
    std::vector<vec4> sample_;


    vec4 operator()(int x, int y, int z){
        return x*basis_[0] + y*basis_[1] + z*basis_[2];
    }

    std::vector<vec4>* MakeSample3d(int X,int Y, int Z){

        for(int x=-X; x<X;x++)
            for(int y=-Y; y<Y;y++)
                for(int z=-Z; z<Z;z++)
                    sample_.emplace_back((*this)(x,y,z));
        LOG("Created Sample");
        return &sample_;
    }

    std::vector<vec4>* MakeSample3d(int sampleInfo[3]){
        return MakeSample3d(sampleInfo[0],sampleInfo[1],sampleInfo[2]);
    }



};

#endif // LATTICE_H
