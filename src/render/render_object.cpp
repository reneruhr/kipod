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

void kipod::RenderObject::Draw(const std::string& layout)
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

void kipod::RenderObject::Setup(const std::string& layout)
{
    assert(HasLayout(layout));
    render_layouts_[layout]->Setup();
}

kipod::RenderLayout *kipod::RenderObject::Layout(const std::string& layout)
{
    return render_layouts_[layout].get();
}

kipod::RenderLayout *kipod::RenderObject::Layout()
{
    if(lay_) return lay_;
    else return nullptr;
}

void kipod::RenderObject::AddLayout(const std::string& name, kipod::RenderLayout&& layout)
{
//    render_layouts_.insert( { name, std::make_unique<decltype(layout)>(std::move(layout)) } );
    layout.AddTo(name, render_layouts_);
    if(lay_==nullptr) lay_ = render_layouts_[name].get();
}

bool kipod::RenderObject::HasLayout(const std::string& name)
{
    return render_layouts_.find(name)!=end(render_layouts_);
}

void kipod::RenderObject::ChangeLayout(const std::string &name, kipod::RenderLayout &&layout)
{
    LOG_ENGINE("Changing Layout {}", name);
    assert(HasLayout(name));
    layout.ChangeTo(name, render_layouts_);
}
