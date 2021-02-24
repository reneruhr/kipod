#include "render_object.h"
#include "../utils/buffer_packing.h"
#include "render_manager.h"


glm::mat4 kipod::RenderObject::Transform() const
{
    return glm::mat4(*world_) * glm::mat4(*local_);
}

const glm::mat4 &kipod::RenderObject::TransformWorld() const
{
    return *world_;
}

const glm::mat4 &kipod::RenderObject::TransformLocal() const
{
    return *local_;
}

void kipod::RenderObject::Draw()
{
    if(lay_)  lay_->Draw();
}

void kipod::RenderObject::Draw(kipod::RenderLayout *layout)
{
    if(layout)  layout->Draw();
}

void kipod::RenderObject::Draw(std::string layout)
{
    assert(HasLayout(layout));
    render_layouts_[layout]->Draw();
}

void kipod::RenderObject::Setup()
{
    if(lay_)  lay_->Setup();
}

void kipod::RenderObject::Setup(kipod::RenderLayout *layout)
{
    if(layout)  layout->Setup();
}

void kipod::RenderObject::Setup(std::string layout)
{
    assert(HasLayout(layout));
    render_layouts_[layout]->Setup();
}

kipod::RenderLayout *kipod::RenderObject::Layout(std::string layout)
{
    return render_layouts_[layout];
}

kipod::RenderLayout *kipod::RenderObject::Layout()
{
    if(lay_) return lay_;
    else return nullptr;
}

void kipod::RenderObject::AddLayout(std::pair<std::string, kipod::RenderLayout *> named_layout)
{
    render_layouts_.insert(named_layout);
    if(lay_==nullptr) lay_ = named_layout.second;
}

void kipod::RenderObject::AddLayout_TEMP(std::pair<std::string, std::unique_ptr<kipod::RenderLayout> >&& named_layout)
{
    render_layouts_TEMP.insert(std::forward<decltype(named_layout)>(named_layout));
}

void kipod::RenderObject::AddLayout(std::string name, kipod::RenderLayout *layout)
{
    render_layouts_.insert({name,layout});
    if(lay_==nullptr) lay_ = layout;
}

bool kipod::RenderObject::HasLayout(std::string name)
{
    return render_layouts_.find(name)!=end(render_layouts_);
}
