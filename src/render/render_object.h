#pragma once
#include "../core.h"

#include "render_buffer.h"
#include "render_shader.h"
#include "render_manager.h"

#include "render_texture.h"
#include "render_primitive.h"
#include "render_material.h"

#include "../engine/engine_transform.h"

struct GLVertex{
    vec3 position_;
    vec3 normal_;
    vec2 texture_;

    GLVertex() = default;
    GLVertex(vec3 position, vec3 normal, vec2 texture)
        : position_(position), normal_(normal), texture_(texture) {}
};


struct GLTriangle{
    GLVertex vertices_[3];

    GLTriangle() = default;
    GLTriangle(GLVertex v, GLVertex w, GLVertex u) {
        vertices_[0] = v;
        vertices_[1] = w;
        vertices_[2] = u;
    }
};

namespace kipod{

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


    glm::mat4 Transform() const
    {
        return glm::mat4(*world_) * glm::mat4(*local_);
    }

    const glm::mat4& TransformWorld() const
    {
        return *world_;
    }
    const glm::mat4& TransformLocal() const
    {
        return *local_;
    }


    virtual void Draw(RenderLayout* layout_ = nullptr);
    virtual void Draw(std::string layout) { assert(HasLayout(layout)); render_layouts_[layout]->Draw(); }
    virtual void Setup(RenderLayout* layout_ = nullptr);
    virtual void Setup(std::string layout){ render_layouts_[layout]->Setup();  }
    virtual RenderLayout* Layout(std::string layout) { return render_layouts_[layout]; }
    virtual void AddLayout(std::pair<std::string, RenderLayout*> named_layout)  {     render_layouts_.insert(named_layout);   }
    virtual void AddLayout(std::string name, RenderLayout* layout)  {     render_layouts_.insert({name,layout});   }
    virtual bool HasLayout(std::string name) { return render_layouts_.find(name)!=end(render_layouts_);}

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
};







}
