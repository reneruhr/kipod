#pragma once
#include "../core.h"
#include "render_buffer.h"
#include "render_shader.h"
#include "render_primitive.h"
#include "render_material.h"
#include "render_layout.h"
#include "../engine/engine_transform.h"

namespace kipod{

class Texture;
class FrameBuffer;


class RenderObject{
public:
    std::unique_ptr<kipod::Transform> world_ = std::make_unique<kipod::Transform>();
    std::unique_ptr<kipod::Transform> local_ = std::make_unique<kipod::Transform>();

    RenderObject() = default;
    RenderObject(RenderObject&&) = default;
    RenderObject& operator=(RenderObject&&) = default;
    RenderObject(RenderObject& other) : world_(new kipod::Transform(*other.world_)),
                                        local_(new kipod::Transform(*other.local_)){}
    RenderObject& operator=(const RenderObject& other) {
        world_.reset(new kipod::Transform( *other.world_ ) );
        local_.reset(new kipod::Transform( *other.local_ ) );
        return *this;
    }

    std::string name_;

    RenderMaterial* mat_ = nullptr;
    Texture* tex_ = nullptr;
    FrameBuffer* fra_ = nullptr;

    std::unordered_map<std::string, RenderLayout*> render_layouts_;
    RenderLayout* lay_ = nullptr;

    glm::mat4 local_transform_ = glm::mat4(1.0);
    glm::mat4 world_transform_ = glm::mat4(1.0);

    glm::mat4 Transform() const;

    const glm::mat4& TransformWorld() const;
    const glm::mat4& TransformLocal() const;


    virtual void Draw();
    virtual void Draw(RenderLayout* layout_);
    virtual void Draw(std::string layout);
    virtual void Setup();
    virtual void Setup(RenderLayout* layout_);
    virtual void Setup(std::string layout);
    virtual RenderLayout* Layout(std::string layout);
    virtual RenderLayout* Layout();
    virtual void AddLayout(std::pair<std::string, RenderLayout*> named_layout);
    virtual void AddLayout(std::string name, RenderLayout* layout);
    virtual bool HasLayout(std::string name);
};


}
