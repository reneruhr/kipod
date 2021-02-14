#pragma once
#include "../core.h"
#include "render_buffer.h"
#include "render_shader.h"
#include "render_primitive.h"
#include "render_material.h"
#include "../engine/engine_transform.h"

namespace kipod{

class Texture;
class FrameBuffer;

class RenderLayout{
    friend class RenderObject;
protected:
    virtual ~RenderLayout() = default;

    RenderMaterial* mat_ = nullptr;
    Texture* tex_ = nullptr;
    FrameBuffer* fra_ = nullptr;

    virtual void Draw()=0;
    virtual void Setup()=0;
};

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
    virtual void AddLayout(std::pair<std::string, RenderLayout*> named_layout);
    virtual void AddLayout(std::string name, RenderLayout* layout);
    virtual bool HasLayout(std::string name);
};

class GLRenderLayout : public RenderLayout{

public:
    ElementsBuffer* ebo_ = nullptr;
    VertexAttributeObject* vao_ = nullptr;
    VertexBuffer* vbo_ = nullptr;
    Shader* sha_ = nullptr;

    virtual void Draw() override;
    virtual void Setup() override;
    void Unbind();

    void SetupColoredTriangles(const std::vector<vec3> *vertices, const std::vector<unsigned int> *indices);
    void SetupColoredTriangles(const std::vector<vec3> *vertices, const std::vector<unsigned int> *indices,
                               const std::vector<vec3> *normals, const std::vector<unsigned int> *nindices);
    void SetupGLTriangles(const std::vector<GLTriangle> *triangles, const std::vector<unsigned int> *indices);
    void SetupShape(const std::vector<vec2>* vertices_);
    void SetupPointSet(const std::vector<vec4>* vertices);
    void SetupLines(const std::vector<vec3> *vertices, const std::vector<vec3> *colors);
    void SetupGrid(const std::vector<vec3> *vertices);
};



}
