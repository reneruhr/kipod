#include "render_scene.h"
#include "render_manager.h"
#include "../utils/image_writer.h"

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

auto kipod::RenderScene::AddRenderObject(std::unique_ptr<RenderObject> object, bool visible) -> kipod::RenderObject*
{
    auto obj_ptr =  render_objects_.emplace_back(std::move(object)).get();
    if(visible) visible_objects_.push_back(obj_ptr);
    return obj_ptr;
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

void kipod::RenderScene::TakeScreenshot(std::string name = "screenshot", bool count = true)
{
   if(frames_per_screenshot_ && !(frame_count_ % frames_per_screenshot_)){
       static const GLuint channels = 4;
       static GLubyte *data = (GLubyte*)malloc(channels * width_ * height_); 
       static const GLenum gl_format = GL_RGBA;
       static unsigned int total_count = 0;
       glReadPixels(0, 0, width_, height_, gl_format, GL_UNSIGNED_BYTE, data);
       static ImageData image = {width_, height_, channels, data};
       name+= std::to_string(total_count);
       if(count) ++total_count;
       ImageWriter::WriteImage(image, name);
   }
   // ffmpeg -r 3 -f image2 -s 600x400 -start_number 0 -i Physical%d.png  -vframes 145 -vcodec libx264 -crf 25  -pix_fmt yuv420p test_long.mp4
}

void kipod::RenderScene::ClearScreen()
{
    visible_objects_.clear();
}
