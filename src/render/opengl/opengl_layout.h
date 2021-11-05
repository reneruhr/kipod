#pragma once
#include "../render_layout.h"
#include "opengl_buffer.h"
#include "Eigen/Dense"

namespace kipod{

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Vec5f = Eigen::Matrix<float,5,1>;
class GLRenderLayout : public RenderLayout{

    template<typename Vector, typename... MoreVectors>
    void AddBufferData(const std::vector<Vector>&, MoreVectors... more_vectors);
    void AddBufferData(GLchar);


public:
    ~GLRenderLayout() override = default;
    std::shared_ptr<ElementsBuffer> ebo_ = nullptr;
    std::shared_ptr<VertexAttributeObject> vao_ = nullptr;
    std::shared_ptr<VertexBuffer> vbo_ = nullptr;
    std::shared_ptr<Shader> sha_ = nullptr;

    void Draw() override;
    void Setup() override;
    void AddTo(const std::string&, std::unordered_map<std::string, std::unique_ptr<RenderLayout>>&) override;
    void ChangeTo(const std::string&, std::unordered_map<std::string, std::unique_ptr<RenderLayout>>&) override;
    void Unbind();

    void SetupColoredTriangles(const std::vector<Vec3> *vertices, const std::vector<unsigned int> *indices);
    void SetupColoredTriangles(const std::vector<Vec3> *vertices, const std::vector<unsigned int> *indices,
                               const std::vector<Vec3> *normals, const std::vector<unsigned int> *nindices);
    void SetupGLTriangles(const std::vector<GLTriangle> *triangles);
    void SetupShape(const std::vector<glm::vec2>* vertices_);
    void SetupPointSet(const std::vector<glm::vec4>* vertices);
    void SetupPointSet23(const std::vector<Vec5f>* vertices);
    void SetupLines(const std::vector<Vec3> *vertices, const std::vector<Vec3> *colors);
    void SetupGrid(const std::vector<Vec3> *vertices);

    template<typename Vector, typename... MoreVectors>
    void SetupLayout(const std::vector<Vector>&, MoreVectors... );
};

}
