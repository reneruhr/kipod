#include "opengl_layout.h"
#include "../render_manager.h"
#include "../../utils/buffer_packing.h"



void kipod::GLRenderLayout::Draw()
{

    vao_->Bind();
    if(ebo_->count_){
        ebo_->Bind();
        glDrawElements(ebo_->primitive_, ebo_->count_, GL_UNSIGNED_INT, (void*)0);
        ebo_->Unbind();
    }else{
        vbo_->Bind();
        glDrawArrays(ebo_->primitive_, 0, vbo_->count_);
        vbo_->Unbind();
    }
    vao_->Unbind();
}

void kipod::GLRenderLayout::Setup()
{
    ebo_->Set();
    vao_->Set();
    vbo_->Bind();
    vao_->SetAttributes();
    ebo_->Unbind();
    vbo_->Unbind();
    vao_->Unbind();
}

void kipod::GLRenderLayout::Unbind()
{
    ebo_->Unbind();
    vbo_->Unbind();
    vao_->Unbind();
}



void kipod::GLRenderLayout::SetupColoredTriangles(const std::vector<vec3>* vertices, const std::vector<unsigned int>* indices,
                                                  const std::vector<vec3>* normals,  const std::vector<unsigned int>* nindices)
{
    LOG_ENGINE("Call: Colored Triangles Setup");
    std::vector<vec3> vnVector;
    std::vector<unsigned int> indices_vector;
    pack_vectors(*vertices, *normals, vnVector, *indices, *nindices, indices_vector);

    ebo_ = std::make_shared<kipod::ElementsBuffer>((void*)indices_vector.data(), indices_vector.size(), indices_vector.size()*sizeof(unsigned int));
    ebo_->Set();

    vao_ = std::make_shared<kipod::VertexAttributeObject>();
    vao_->Set();

    unsigned int buffersize = vnVector.size()*sizeof(vec3);
    vbo_ = std::make_shared<kipod::VertexBuffer>(nullptr, buffersize);
    vbo_->Add(0, buffersize, (void*)vnVector.data());

    vbo_->Bind();

    vao_->Add({0,3,2*sizeof(vec3),0});
    vao_->Add({1,3,2*sizeof(vec3), sizeof(vec3)});
    vao_->SetAttributes();

    Unbind();
}

void kipod::GLRenderLayout::SetupColoredTriangles(const std::vector<vec3> *vertices, const std::vector<unsigned int> *indices_vector)
{
    LOG_ENGINE("Call: Colored Triangles Without Normals Setup");
    ebo_ = std::make_shared<kipod::ElementsBuffer>((void*)indices_vector->data(), indices_vector->size(), indices_vector->size()*sizeof(unsigned int));
    ebo_->Set();

    vao_ = std::make_shared<kipod::VertexAttributeObject>();
    vao_->Set();

    unsigned int buffersize = vertices->size()*sizeof(vec3);
    vbo_ = std::make_shared<kipod::VertexBuffer>(nullptr, buffersize);
    vbo_->Add(0, buffersize, (void*)vertices->data());

    vbo_->Bind();

    vao_->Add({0,3,sizeof(vec3),0});
    vao_->SetAttributes();

    Unbind();
}

void kipod::GLRenderLayout::SetupGLTriangles(const std::vector<GLTriangle>* triangles, const std::vector<unsigned int>* indices)
{
    LOG_ENGINE("Call: Textured Triangles Setup");

    ebo_ = std::make_shared<kipod::ElementsBuffer>((void*)indices->data(), indices->size(), indices->size()*sizeof(unsigned int));
    ebo_->Set();

    vao_ = std::make_shared<kipod::VertexAttributeObject>();
    vao_->Set();

    unsigned int buffersize = triangles->size()*sizeof(GLTriangle);
    vbo_ = std::make_shared<kipod::VertexBuffer>(nullptr, buffersize);
    vbo_->Add(0, buffersize, (void*)triangles->data());

    vbo_->Bind();

    vao_->Add({0,3,sizeof(GLVertex),0});
    vao_->Add({1,3,sizeof(GLVertex), offsetof(GLVertex, normal_)});
    vao_->Add({2,2,sizeof(GLVertex), offsetof(GLVertex, texture_)});
    vao_->SetAttributes();

    Unbind();
}

void kipod::GLRenderLayout::SetupShape(const std::vector<vec2> *vertices)
{
    LOG_ENGINE("Call: Shape Setup");

    auto indices_vector = std::vector<unsigned int>(std::size(*vertices));
    std::iota(std::begin(indices_vector), std::end(indices_vector), 0);

    ebo_ = std::make_shared<kipod::ElementsBuffer>((void*)indices_vector.data(), indices_vector.size(), indices_vector.size()*sizeof(unsigned int));
    ebo_->primitive_ = GL_TRIANGLE_FAN;
    ebo_->Set();

    vao_ = std::make_shared<kipod::VertexAttributeObject>();
    vao_->Set();

    unsigned int buffersize = vertices->size()*sizeof(vec2);
    vbo_ = std::make_shared<kipod::VertexBuffer>(nullptr, buffersize);
    vbo_->Add(0, buffersize, (void*)vertices->data());
    vbo_->Bind();

    vao_->Add({0,2,sizeof(vec2),0});
    vao_->SetAttributes();

    Unbind();
}

void kipod::GLRenderLayout::SetupPointSet(const std::vector<vec4> *vertices)
{
    LOG_ENGINE("Call: PointSet Setup");
    SetupLayout(*vertices, GL_POINTS);

}

void kipod::GLRenderLayout::SetupLines(const std::vector<vec3> *vertices, const std::vector<vec3> *colors)
{
    LOG_ENGINE("Call: Lines Setup");
    SetupLayout(*vertices, *colors, GL_LINES);

}

void kipod::GLRenderLayout::SetupGrid(const std::vector<vec3> *vertices)
{
    LOG_ENGINE("Call: Grid Setup");
    SetupLayout(*vertices, GL_LINES);

}

unsigned long  CalculateBufferSize(GLchar){     return 0;   }

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

template<typename Vector, typename... MoreVectors>
void kipod::GLRenderLayout::AddBufferData(const std::vector<Vector>& vectors, MoreVectors... more_vectors)
{
    unsigned long buffersize = vectors.size()*sizeof(Vector);
    vbo_->count_ = vectors.size();
    vbo_->Add(buffersize, (void*)vectors.data());
    vao_->Add({vao_->NumberOfAttributes(), Vector::Length(), sizeof(Vector),0});

    AddBufferData(more_vectors...);
}

void kipod::GLRenderLayout::AddBufferData(GLchar primitive)
{
    ebo_ = std::make_shared<kipod::ElementsBuffer>();
    ebo_->primitive_ = primitive;
}

template<typename Vector, typename... MoreVectors>
void  kipod::GLRenderLayout::SetupLayout(const std::vector<Vector>& vectors, MoreVectors... more_vectors)
{
    vao_ = std::make_shared<kipod::VertexAttributeObject>();
    vao_->Set();
    unsigned long totalbuffersize = CalculateBufferSize(vectors, more_vectors...);
    vbo_ = std::make_shared<kipod::VertexBuffer>(nullptr, totalbuffersize);
    AddBufferData(vectors,  more_vectors...);
    vbo_->Bind();
    vao_->SetAttributes();
    Unbind();
}

