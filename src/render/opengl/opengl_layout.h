#pragma once
#include "../render_layout.h"
#include "opengl_buffer.h"


namespace kipod{

class GLRenderLayout : public RenderLayout{

    template<typename Vector, typename... MoreVectors>
    void AddBufferData(const std::vector<Vector>&, MoreVectors... more_vectors);
    void AddBufferData(GLchar);


public:
    ~GLRenderLayout() = default;
    std::shared_ptr<ElementsBuffer> ebo_ = nullptr;
    std::shared_ptr<VertexAttributeObject> vao_ = nullptr;
    std::shared_ptr<VertexBuffer> vbo_ = nullptr;
    std::shared_ptr<Shader> sha_ = nullptr;

    virtual void Draw() override;
    virtual void Setup() override;
    virtual void AddTo(const std::string&, std::unordered_map<std::string, std::unique_ptr<RenderLayout>>&) override;
    void Unbind();

    void SetupColoredTriangles(const std::vector<vec3> *vertices, const std::vector<unsigned int> *indices);
    void SetupColoredTriangles(const std::vector<vec3> *vertices, const std::vector<unsigned int> *indices,
                               const std::vector<vec3> *normals, const std::vector<unsigned int> *nindices);
    void SetupGLTriangles(const std::vector<GLTriangle> *triangles);
    void SetupShape(const std::vector<glm::vec2>* vertices_);
    void SetupPointSet(const std::vector<glm::vec4>* vertices);
    void SetupLines(const std::vector<vec3> *vertices, const std::vector<vec3> *colors);
    void SetupGrid(const std::vector<vec3> *vertices);

    template<typename Vector, typename... MoreVectors>
    void SetupLayout(const std::vector<Vector>&, MoreVectors... );
};

}
