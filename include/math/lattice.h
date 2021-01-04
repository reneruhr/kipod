#ifndef LATTICE_H
#define LATTICE_H

#include "core.h"

class Lattice
{
public:
    Lattice(vec3 basis3d[3]): dim(3){
        for(int i = 0;i<3;i++) basis[i]=vec4(basis3d[i],0);
    }
    Lattice(mat3& basis3d): dim(3){
        for(int i = 0;i<3;i++) basis[i]=vec4(basis3d[i],0);
    }
    Lattice(mat4& basis4d, int dim=4): dim(dim),basis(basis4d){}
    Lattice(int dim=4): dim(dim), basis(mat4(1.0)){}

    mat4 getBasisMatrix(){
        return basis;
    }

    int dim;
    mat4 basis;
    std::vector<vec4> sample;


    vec4 operator()(int x, int y, int z){
        return x*basis[0] + y*basis[1] + z*basis[2];
    }

    std::vector<vec4>* makeSample3d(int X,int Y, int Z){

        for(int x=-X; x<X;x++)
            for(int y=-Y; y<Y;y++)
                for(int z=-Z; z<Z;z++)
                    sample.emplace_back((*this)(x,y,z));
        LOG("Created Sample");
        return &sample;
    }

    std::vector<vec4>* makeSample3d(int sampleInfo[3]){
        return makeSample3d(sampleInfo[0],sampleInfo[1],sampleInfo[2]);
    }



};

#endif // LATTICE_H
