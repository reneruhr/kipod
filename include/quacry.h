#ifndef QUACRY_H
#define QUACRY_H

#include "utils/mat.h"
#include <vector>

class Scene;
class PointSet;
class PrimMeshModel;
class Camera;

struct WindowBox
{
    WindowBox(): window_size_{-10,10,-10,10,-1,1,-1,1} {};

    std::vector< float > window_size_;

    float operator[](int i){
        return window_size_[i];
    }

    float Left(){
        return (*this)[0];
    }
    float Right(){
        return (*this)[1];
    }
    float Bottom(){
        return (*this)[2];
    }
    float Top(){
        return (*this)[3];
    }
    float Near(){
        return (*this)[4];
    }
    float Far(){
        return (*this)[5];
    }
    float Near2(){
        return (*this)[6];
    }
    float Far2(){
        return (*this)[7];
    }

};

class QuaCry
{
    PointSet* pointset_;
    WindowBox window_box_;

    Scene* scene_;
    Camera* camera_;
    //PrimMeshModel* window_box_;


public:
    QuaCry(Scene* scene);
};

#endif // QUACRY_H
