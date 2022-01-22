#pragma once
#include "../render_layout.h"
#include "opengl_buffer.h"
#include "Eigen/Dense"
#include "opengl_engine.h"
#include "opengl_buffer_410.h"
#include "opengl_buffer_450.h"

namespace kipod{

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Vec5f = Eigen::Matrix<float,5,1>;
using Vec10f = Eigen::Matrix<float,10,1>;

template <typename Vector>
unsigned int VectorLength(const Vector& vector) {
    return std::size(vector);
}
template<> unsigned int VectorLength(const glm::vec2&);
template<> unsigned int VectorLength(const glm::vec3&);
template<> unsigned int VectorLength(const glm::vec4&);

inline unsigned int GetActiveObjectLength(const glm::vec4&){ return 4;}
inline unsigned int GetActiveObjectLength(const vec2&){ return 2;}
inline unsigned int GetActiveObjectLength(const vec3&){ return 3;}


inline unsigned long  CalculateBufferSize(GLchar){     return 0;   }

template<typename Vector, typename... MoreVectors>
unsigned long  CalculateBufferSize(const std::vector<Vector>& vectors, MoreVectors... more_vectors)
{
    return vectors.size()*sizeof(Vector)+CalculateBufferSize(more_vectors...);
}

template<typename Vector, typename... MoreVectors>
unsigned long  AddBuffer(const std::vector<Vector>& vectors, MoreVectors... more_vectors)
{
    return vectors->size()*sizeof(Vector)+CalculateBufferSize(more_vectors...);
}

class GLRenderLayout : public RenderLayout
{
    template<typename Vector, typename... MoreVectors>
    void AddBufferData(const std::vector<Vector>& vectors, MoreVectors... more_vectors)
    {
        unsigned long buffersize = vectors.size()*sizeof(Vector);
        vbo_->count_ = vectors.size();

        vbo_->Add(buffersize, (void*)vectors.data());
        vao_->Add({vao_->NumberOfAttributes(), VectorLength(vectors[0]), sizeof(Vector),0});

        AddBufferData(more_vectors...);
    }
    void AddBufferData(GLchar);

public:
    ~GLRenderLayout() override = default;
    std::shared_ptr<ElementsBuffer> ebo_ = nullptr;
    std::shared_ptr<VertexAttributeObject> vao_ = nullptr;
    std::shared_ptr<VertexBuffer> vbo_ = nullptr;
    std::shared_ptr<Shader> sha_ = nullptr;

    void Draw() override;
    void Setup() override;
    void SetSubIndex(unsigned int count, std::size_t start);
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
    void SetupEdges23(const std::vector<Vec10f>* vertices);
    void SetupLines(const std::vector<Vec3> *vertices, const std::vector<Vec3> *colors);
    void SetupGrid(const std::vector<Vec3> *vertices);

    template<typename Vector, typename... MoreVectors>
    void  SetupLayout(const std::vector<Vector>& vectors, MoreVectors... more_vectors)
    {
        vao_ = std::make_shared<kipod::VertexAttributeObject>();
        vao_->Set();
        unsigned long totalbuffersize = CalculateBufferSize(vectors, more_vectors...);

        if(OpenGLEngine::Version()==450){
            vbo_ = std::make_shared<kipod::VertexBuffer450>(nullptr, totalbuffersize);
        }else{
            vbo_ = std::make_shared<kipod::VertexBuffer410>(nullptr, 0, totalbuffersize);
        }
        AddBufferData(vectors,  more_vectors...);

        vbo_->Bind();
        vao_->SetAttributes();
        Unbind();
    }

    template<typename Vector, typename... MoreVectors>
    void  SetupLayout(const std::vector<unsigned int>& indices, const std::vector<Vector>& vectors, MoreVectors... more_vectors)
    {
        LOG_ENGINE("Call: Indexed SetupLayout");
        ebo_ = std::make_shared<kipod::ElementsBuffer>((void*)indices.data(), indices.size(), indices.size()*sizeof(unsigned int));
        ebo_->Set();

        SetupLayout(vectors, std::forward<MoreVectors>(more_vectors)...);
    }

};

}
