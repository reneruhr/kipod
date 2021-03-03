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
    RenderCamera* active_camera_ = nullptr;
    RenderObject* active_render_object_ = nullptr;

    int min_width_ = 100;
    int min_height_ = 100;

    std::vector<std::unique_ptr<RenderObject> > render_objects_;
    std::vector<std::unique_ptr<RenderCamera> > cameras_;
    std::vector<std::unique_ptr<RenderLight> > lights_;

    std::string name_;
    unsigned int width_, height_;
    bool fixed_ratio_ = true;
    float ratio_;
    float reverse_ratio_;
    std::shared_ptr<kipod::FrameBuffer> framebuffer_;

public:
    RenderScene() = default;
    RenderScene(int w, int h);
    RenderScene(RenderScene&) = default;
    RenderScene& operator=(RenderScene&) = default;
    RenderScene(RenderScene&&) = default;
    RenderScene& operator=(RenderScene&&) = default;
    virtual ~RenderScene() = default;

    virtual void Setup() = 0;
    virtual void Draw() = 0;

    virtual void DrawGui(){}

    virtual void Resize(int w, int h);
    virtual unsigned int SceneAsFramebuffer();

    void AddLight(RenderLight&& light);
    void AddCamera(RenderCamera&& camera);
    void AddRenderObject(RenderObject&& model);

    RenderCamera* GetActiveCamera();
    RenderObject* GetActiveRenderObject();

    int NumberOfCameras();
    int NumberOfRenderObjects();
    bool HasLight() { return !lights_.empty(); }
    bool HasRenderObject() { return !render_objects_.empty(); }

    void SetActiveCamera(int id);
    void SetActiveRenderObject(int id);

    void SwapFrameBuffer();
};


}//kipod
