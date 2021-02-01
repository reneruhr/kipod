#ifndef QUACRY_H
#define QUACRY_H

#include "utils/mat.h"
#include <vector>

#include "../include/guimodule.h"
#include "guimathcontrol.h"

#include "pointset.h"
#include "shapes.h"

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

class QuaCry : public PointSet,
               public WindowBox,
               public GUIModule,
               GUIMathControl,
               public Shape
{
    Scene* scene_;
    Camera* camera_;
    Camera* sideViewCamera_;

    WindowType window_type_;

public:
    QuaCry(Scene* scene, mat4 basis,
           std::vector< float > window_size, std::vector< int > sample_size,
           WindowType type = WindowType::Box, Shape shape = Shape(Square()));
    QuaCry(Scene* scene);

    virtual void Init() override;
    virtual void Draw() override;


    void DrawWindow(GLRenderer *glrenderer);

    void SetOutsideVisibility(float alpha){
        lattice_data_->alpha_ = alpha;
    }
    void SetColorZW(float* z, float* w){
        lattice_data_->z_color_ = vec4(z);
        lattice_data_->w_color_ = vec4(w);
    }
};

#endif // QUACRY_H
