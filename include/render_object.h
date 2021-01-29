#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <vector>
#include <string>

#include "render_buffer.h"
#include "render_shader.h"
#include "render_manager.h"

#include "texture.h"
#include "render_primitive.h"
#include "render_material.h"

#include "../include/utils/mat.h"

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
    RenderObjectType type_;

    RenderMaterial* mat_ = nullptr;
    Texture* tex_ = nullptr;
    FrameBuffer* fra_ = nullptr;

    std::unordered_map<std::string, RenderLayout*> render_layouts_;
    RenderLayout* lay_ = nullptr;

    glm::mat4 local_transform_ = glm::mat4(1.0);
    glm::mat4 world_transform_ = glm::mat4(1.0);

    glm::mat4 Transform() const
    {
        return world_transform_*local_transform_;
    }

    void Draw(RenderLayout* layout_ = nullptr);
    void Draw(std::string layout) { render_layouts_[layout]->Draw(); }
    void Setup(RenderLayout* layout_ = nullptr);
    void Setup(std::string layout){ render_layouts_[layout]->Setup();  }
    RenderLayout* Layout(std::string layout) { return render_layouts_[layout]; }
    void AddLayout(std::pair<std::string, RenderLayout*> named_layout)  {     render_layouts_.insert(named_layout);   }
    void AddLayout(std::string name, RenderLayout* layout)  {     render_layouts_.insert({name,layout});   }
    bool HasLayout(std::string name) { return render_layouts_.find(name)!=end(render_layouts_);}

};




class GLRenderLayout : public RenderLayout{
public:
//    GLRenderLayout() : RenderLayout()
//    {
//        LOG_ENGINE("Call: GLRenderLayout Constructor.");
//    }
    //GLRenderLayout(const GLRenderLayout& layout) = default;

    ElementsBuffer* ebo_ = nullptr;
    VertexAttributeObject* vao_ = nullptr;
    VertexBuffer* vbo_ = nullptr;
    Shader* sha_ = nullptr;

    virtual void Draw() override;
    virtual void Setup() override;
    void Unbind();

    void SetupColoredTriangles(const std::vector<vec3> *vertices, const std::vector<unsigned int> *indices,
                               const std::vector<vec3> *normals, const std::vector<unsigned int> *nindices);
    void SetupGLTriangles(const std::vector<GLTriangle> *triangles, const std::vector<unsigned int> *indices);

};







}

#endif // RENDEROBJECT_H
