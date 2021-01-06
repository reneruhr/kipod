#ifndef QUACRY_H
#define QUACRY_H

#include "utils/mat.h"
#include <vector>

#include "../include/guimodule.h"
#include "guimathcontrol.h"

#include "pointset.h"

class Scene;
class PrimMeshModel;
class Camera;


struct WindowBox
{
    WindowBox(): window_size_{-10,10,-10,10,-3,3,-3,3} {}
    WindowBox(std::vector< float > window_size): window_size_(window_size) {}

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

class QuaCry : public PointSet, public WindowBox, public GUIModule, GUIMathControl
{
    Scene* scene_;
    Camera* camera_;
    Camera* sideViewCamera_;
    //PrimMeshModel* window_box_;

    mat4 current_transform_;
public:
    QuaCry(Scene* scene, mat4 basis, std::vector< float > window_size, std::vector< int > sample_size );
    QuaCry(Scene* scene);

    void Init();
    virtual void Draw() override;
};

#endif // QUACRY_H
