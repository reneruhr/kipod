#pragma once
#include "../core.h"
#include "render_object.h"
#include "render_camera.h"
#include "render_light.h"

namespace kipod {
using LightContainer = std::vector<std::unique_ptr<RenderLight> >;
using ObjectContainer = std::vector<std::unique_ptr<RenderObject>>;
using CameraContainer = std::vector<std::unique_ptr<RenderCamera> >;
using VisibleContainer = std::vector<RenderObject*>;

class RenderScene {
    friend class Gui;

protected:
    RenderCamera* active_camera_ = nullptr;
    RenderObject* active_render_object_ = nullptr;

    int min_width_ = 100;
    int min_height_ = 100;

    ObjectContainer render_objects_;
    VisibleContainer visible_objects_;
    CameraContainer cameras_;
    LightContainer lights_;

    std::string name_;
    int width_, height_;
    bool fixed_ratio_ = true;
    float ratio_;
    float reverse_ratio_;
    std::shared_ptr<kipod::FrameBuffer> framebuffer_ = nullptr;
    std::unordered_map<std::string, std::shared_ptr<kipod::FrameBuffer>> framebuffers_;

    int frame_count_ = 0;
    int frames_per_screenshot_ = 0;
public:
    RenderScene() = default;
    RenderScene(int w, int h);
    RenderScene(RenderScene&) = default;
    RenderScene& operator=(const RenderScene&) = default;
    RenderScene(RenderScene&&) = default;
    RenderScene& operator=(RenderScene&&) = default;
    virtual ~RenderScene() = default;

    virtual void Setup() = 0;
    virtual void Draw() = 0;
    virtual void Update(){}

    virtual void DrawGui(){}
    void TakeScreenshot(std::string, bool);

    virtual void Resize(int w, int h);
    virtual unsigned int SceneAsFramebuffer(const std::string& name);

    void SetFixedRatio(bool ratio) { fixed_ratio_ = ratio; }

    void AddLight(RenderLight&& light);
    void AddCamera(RenderCamera&& camera);
    auto AddRenderObject(std::unique_ptr<RenderObject> object, bool visible = true) -> RenderObject*;

    RenderCamera* GetActiveCamera();
    RenderObject* GetActiveRenderObject();

    RenderCamera* GetCamera(int id);

    int NumberOfCameras();
    int NumberOfRenderObjects();
    bool HasLight() { return !lights_.empty(); }
    bool HasRenderObject() { return !render_objects_.empty(); }

    void ClearScreen();

    void SetActiveCamera(int id);
    void SetActiveRenderObject(int id);
    void SetActiveRenderObject(RenderObject* object) { active_render_object_ = object; }

    void SwapFrameBuffer();

    int GetFrameCount() const { return frame_count_; }
    int GetFramesPerScreenshot() const { return frames_per_screenshot_; }
    void SetFramesPerScreenshot(int n) { frames_per_screenshot_ = n; }
};
}//kipod
