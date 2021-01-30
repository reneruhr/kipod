#ifndef RENDERSCENE_H
#define RENDERSCENE_H

#include "render_object.h"
#include "render_camera.h"
#include "render_light.h"
#include "core.h"

namespace kipod
{

class RenderScene
{
    friend class GUI;


protected:
    std::vector<RenderObject*> render_objects_;
    std::vector<RenderCamera*> cameras_;
    std::vector<RenderLight*> lights_;

    unsigned int width_,height_;
    entt::registry registry;

public:
    RenderScene() = default;
    RenderScene(unsigned int w, unsigned int h) : width_(w), height_(h) {}
    virtual ~RenderScene() = default;

    virtual void Setup(){}
    virtual void Draw(){}

    virtual void Resize(unsigned int w, unsigned int h){ width_ = w; height_ = h; }


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
