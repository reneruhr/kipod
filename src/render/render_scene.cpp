#include "render_scene.h"
#include "render_manager.h"

kipod::RenderScene::RenderScene(int w, int h) : width_(w), height_(h), ratio_(float(w)/float(h)), reverse_ratio_(float(h)/float(w))
{
    LOG_ENGINE("Create Scene with width {} and height {}", w, h);
    framebuffer_ =  std::make_shared<kipod::FrameBuffer>(width_, height_);
}

void kipod::RenderScene::Resize(int w,int h) {
    if(fixed_ratio_) h =reverse_ratio_* w; //h = 1/ratio_*w;
    width_ = w; height_ = h;
    LOG_ENGINE("Resized Scene. Width {} and height {}", w, h);
    framebuffer_->Resize(width_,height_);
}

unsigned int kipod::RenderScene::SceneAsFramebuffer()
{
   return framebuffer_->FrameBufferAsTexture();
}

void kipod::RenderScene::AddLight(kipod::RenderLight *light){
    lights_.emplace_back(light);
}

void kipod::RenderScene::AddCamera(kipod::RenderCamera *camera){
    cameras_.emplace_back(camera);
}

void kipod::RenderScene::AddModel(kipod::RenderObject *model){
    render_objects_.emplace_back(model);
}
