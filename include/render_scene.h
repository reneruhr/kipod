#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include "render_engine.h"
#include <vector>

namespace kipod
{

class RenderScene
{
protected:
    std::vector<RenderObject*> render_objects_;
    std::vector<RenderCamera> cameras_;
    std::vector<RenderLight> lights_;

public:
    RenderScene(){}

    virtual void Setup(){}
    virtual void Draw(){}

    void AddLight(RenderLight light){
        lights_.emplace_back(light);
    }

    void AddCamera(RenderCamera camera){
        cameras_.emplace_back(camera);
    }
};


}//kipod
#endif // RENDERSCENE_H
