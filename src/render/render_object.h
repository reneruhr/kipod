#pragma once
#include <memory>

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
private:
    std::unordered_map<std::string, std::unique_ptr<RenderLayout>> render_layouts_;
    RenderLayout* lay_ = nullptr;
public:
    std::unique_ptr<kipod::Transform> world_ = std::make_unique<kipod::Transform>();
    std::unique_ptr<kipod::Transform> local_ = std::make_unique<kipod::Transform>();

    RenderObject() = default;
    RenderObject(RenderObject&&) = default;
    RenderObject& operator=(RenderObject&&) = default;
    RenderObject(const RenderObject& other) : world_(new kipod::Transform(*other.world_)),
                                        local_(new kipod::Transform(*other.local_)){}
    RenderObject& operator=(const RenderObject& other) {
        world_ = std::make_unique<kipod::Transform>( *other.world_ );
        local_ = std::make_unique<kipod::Transform>( *other.local_ );
        return *this;
    }
    virtual ~RenderObject() = default;

    std::string name_;

    std::shared_ptr<RenderMaterial> mat_ = nullptr;
    std::shared_ptr<Texture> tex_ = nullptr;
    std::shared_ptr<FrameBuffer> fra_ = nullptr;

    glm::mat4 Transform() const;

    const glm::mat4& TransformWorld() const;
    const glm::mat4& TransformLocal() const;

    virtual void Draw();
    virtual void Draw(RenderLayout* layout);
    virtual void Draw(const std::string& layout);
    virtual void Setup();
    virtual void Update(){}
    virtual void Setup(RenderLayout* layout);
    virtual void Setup(const std::string& layout);
    virtual RenderLayout* Layout(const std::string& layout);
    virtual RenderLayout* Layout();
    virtual void AddLayout(const std::string& name, RenderLayout&& layout);
    virtual void ChangeLayout(const std::string& name, RenderLayout&& layout);
    virtual bool HasLayout(const std::string& name);

};


}
