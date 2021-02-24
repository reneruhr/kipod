#pragma once
#include "../render_layout.h"

namespace kipod{

class GLRenderLayout : public RenderLayout{

    template<typename Vector, typename... MoreVectors>
    void AddBufferData(const std::vector<Vector>&, MoreVectors... more_vectors);
    void AddBufferData(GLchar);

public:
    std::shared_ptr<ElementsBuffer> ebo_ = nullptr;
    std::shared_ptr<VertexAttributeObject> vao_ = nullptr;
    std::shared_ptr<VertexBuffer> vbo_ = nullptr;
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

    template<typename Vector, typename... MoreVectors>
    void SetupLayout(const std::vector<Vector>&, MoreVectors... );


};



}
