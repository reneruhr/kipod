#include "render_scene.h"
#include "render_manager.h"

kipod::RenderScene::RenderScene(int w, int h) : width_(w), height_(h), ratio_(float(w)/float(h)), reverse_ratio_(float(h)/float(w))
{
    LOG_DEBUG("Create Scene with width {} and height {}", w, h);
    framebuffer_ =  std::make_shared<kipod::FrameBuffer>(width_, height_);
    framebuffers_= {{"Module Viewport", framebuffer_}};
}

void kipod::RenderScene::Resize(int w,int h)
{
    w = std::max(w, min_width_);
    h = std::max(h, min_height_);
    if(fixed_ratio_) h = std::floor(reverse_ratio_* w); //h = 1/ratio_*w;
    width_ = w; height_ = h;
    LOG_DEBUG("Resized Scene. Width {} and height {}", w, h);
    // framebuffer_->Resize(width_,height_);
    for(auto& fb : framebuffers_) fb.second->Resize(width_,height_);
}

unsigned int kipod::RenderScene::SceneAsFramebuffer(const std::string& name )
{
   return framebuffers_[name]->FrameBufferAsTexture();
}

void kipod::RenderScene::AddLight(kipod::RenderLight&& light)
{
    lights_.push_back(
                std::make_unique<kipod::RenderLight>(
                    std::forward<kipod::RenderLight>(light)));
}

void kipod::RenderScene::AddCamera(kipod::RenderCamera&& camera)
{
    cameras_.push_back(
                std::make_unique<kipod::RenderCamera>(
                    std::forward<kipod::RenderCamera>(camera)));
}

void kipod::RenderScene::AddRenderObject(kipod::RenderObject&& object)
{
    render_objects_.push_back(
                std::make_unique<kipod::RenderObject>(
                    std::forward<kipod::RenderObject>(object)));
}

kipod::RenderCamera *kipod::RenderScene::GetActiveCamera()
{
    return active_camera_;
}

kipod::RenderObject *kipod::RenderScene::GetActiveRenderObject()
{
    return active_render_object_;
}

int kipod::RenderScene::NumberOfCameras()
{
    return cameras_.size();
}

int kipod::RenderScene::NumberOfRenderObjects()
{
    return render_objects_.size();
}

void kipod::RenderScene::SetActiveCamera(int id)
{
    if(id<NumberOfCameras()) active_camera_=cameras_[id].get();
}

kipod::RenderCamera* kipod::RenderScene::GetCamera(int id)
{
    if(id<NumberOfCameras()) return cameras_[id].get();
    else return nullptr;
}

void kipod::RenderScene::SetActiveRenderObject(int id)
{
    if(id<NumberOfRenderObjects()) active_render_object_=render_objects_[id].get();
}
