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
    std::vector<RenderCamera*> cameras_;
    std::vector<RenderLight*> lights_;

public:
    RenderScene(){}
    virtual ~RenderScene() = default;

    virtual void Setup(){}
    virtual void Draw(){}



    void AddLight(RenderLight* light){
        lights_.emplace_back(light);
    }

    void AddCamera(RenderCamera* camera){
        cameras_.emplace_back(camera);
    }

    void AddModel(RenderObject* model){
        render_objects_.emplace_back(model);
    }





};


}//kipod
#endif // RENDERSCENE_H
