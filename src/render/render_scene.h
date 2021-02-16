#pragma once

#include "../core.h"
#include "render_object.h"
#include "render_camera.h"
#include "render_light.h"


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
    float reverse_ratio_;
    std::shared_ptr<kipod::FrameBuffer> framebuffer_;


public:


    RenderScene() = default;
    RenderScene(int w, int h);
    virtual ~RenderScene() = default;

    virtual void Setup() = 0;
    virtual void Draw() = 0;

    virtual void DrawGui(){}

    virtual void Resize(int w, int h);
    virtual unsigned int SceneAsFramebuffer();

    void AddLight(RenderLight* light);
    void AddCamera(RenderCamera* camera);
    void AddModel(RenderObject* model);

    void SwapFrameBuffer();
};


}//kipod
