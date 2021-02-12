#pragma once

#include "render_object.h"
#include "render_camera.h"
#include "render_light.h"
#include "../core.h"

namespace kipod
{

class RenderScene
{

friend class Gui;


protected:
    std::vector<RenderObject*> render_objects_;
    std::vector<RenderCamera*> cameras_;
    std::vector<RenderLight*> lights_;

    std::string name_;
    unsigned int width_, height_;
    bool fixed_ratio_ = true;
    float ratio_;
    std::shared_ptr<kipod::FrameBuffer> framebuffer_;


public:


    RenderScene() = default;
    RenderScene(unsigned int w, unsigned int h) : width_(w), height_(h), ratio_(float(w)/float(h))
    {
        LOG_ENGINE("Create Scene with width {} and height {}", w, h);

        framebuffer_ =  std::make_shared<kipod::FrameBuffer>(width_, height_);
    }
    virtual ~RenderScene() = default;

    virtual void Setup(){}
    virtual void Draw(){}

    virtual void DrawGui(){}

    virtual void Resize(unsigned int w, unsigned int h) {
        if(fixed_ratio_) h = 1/ratio_*w;
        width_ = w; height_ = h;
        LOG_ENGINE("Resized Scene. Width {} and height {}", w, h);
        framebuffer_->Resize(width_,height_);
     };
    virtual unsigned int SceneAsFramebuffer() { return framebuffer_->FrameBufferAsTexture(); };



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
