#include "opengl_layout.h"
#include "../render_manager.h"
#include "../../utils/buffer_packing.h"
#include "opengl_engine.h"
#include "opengl_buffer_410.h"
#include "opengl_buffer_450.h"

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

void kipod::GLRenderLayout::AddTo(const std::string& name, std::unordered_map<std::string, std::unique_ptr<kipod::RenderLayout> > &map)
{
    map.insert( { name, std::make_unique<kipod::GLRenderLayout>(std::move(*this)) } );
}

void kipod::GLRenderLayout::ChangeTo(const std::string& name, std::unordered_map<std::string, std::unique_ptr<kipod::RenderLayout>>& map)
{
   LOG_ENGINE("Changing Layout at {}", name);
   map[name] = std::make_unique<kipod::GLRenderLayout>(std::move(*this));
}

void kipod::GLRenderLayout::Unbind()
{
    ebo_->Unbind();
    vbo_->Unbind();
    vao_->Unbind();
}

void kipod::GLRenderLayout::SetupColoredTriangles(const std::vector<Vec3>* vertices, const std::vector<unsigned int>* indices,
                                                  const std::vector<Vec3>* normals,  const std::vector<unsigned int>* nindices)
{
    LOG_ENGINE("Call: Colored Triangles Setup");
    std::vector<Vec3> vnVector;
    std::vector<unsigned int> indices_vector;
    pack_vectors(*vertices, *normals, vnVector, *indices, *nindices, indices_vector);

    ebo_ = std::make_shared<kipod::ElementsBuffer>((void*)indices_vector.data(), indices_vector.size(), indices_vector.size()*sizeof(unsigned int));
    ebo_->Set();

    vao_ = std::make_shared<kipod::VertexAttributeObject>();
    vao_->Set();

    unsigned int buffersize = vnVector.size()*sizeof(Vec3);
    if(OpenGLEngine::Version()==450){
        vbo_ = std::make_shared<kipod::VertexBuffer450>(nullptr, buffersize);
    }else{
        vbo_ = std::make_shared<kipod::VertexBuffer410>(nullptr, vnVector.size(), buffersize);
    }
    vbo_->Add(0, buffersize, (void*)vnVector.data());
    vbo_->Bind();

    vao_->Add({0,3,2*sizeof(Vec3),0});
    vao_->Add({1,3,2*sizeof(Vec3), sizeof(Vec3)});
    vao_->SetAttributes();

    Unbind();
}

void kipod::GLRenderLayout::SetupColoredTriangles(const std::vector<Vec3> *vertices, const std::vector<unsigned int> *indices_vector)
{
    LOG_ENGINE("Call: Colored Triangles Without Normals Setup");
    ebo_ = std::make_shared<kipod::ElementsBuffer>((void*)indices_vector->data(), indices_vector->size(), indices_vector->size()*sizeof(unsigned int));
    ebo_->Set();

    vao_ = std::make_shared<kipod::VertexAttributeObject>();
    vao_->Set();

    unsigned int buffersize = vertices->size()*sizeof(Vec3);

    if(OpenGLEngine::Version()==450){
        vbo_ = std::make_shared<kipod::VertexBuffer450>(nullptr, buffersize);
    }else{
        vbo_ = std::make_shared<kipod::VertexBuffer410>(nullptr, vertices->size(), buffersize);
    }
    vbo_->Add(0, buffersize, (void*)vertices->data());
    vbo_->Bind();

    vao_->Add({0,3,sizeof(Vec3),0});
    vao_->SetAttributes();

    Unbind();
}

void kipod::GLRenderLayout::SetupGLTriangles(const std::vector<GLTriangle>* triangles)
{
    LOG_ENGINE("Call: Textured Triangles Setup");
    unsigned long totalbuffersize = triangles->size()*sizeof(GLTriangle);

    AddBufferData(GL_TRIANGLES);

    vao_ = std::make_shared<kipod::VertexAttributeObject>();
    vao_->Set();

    if(OpenGLEngine::Version()==450){
        vbo_ = std::make_shared<kipod::VertexBuffer450>(nullptr, totalbuffersize);
        vbo_->count_ = triangles->size()*3;
    }else{
        vbo_ = std::make_shared<kipod::VertexBuffer410>(nullptr, triangles->size()*3, totalbuffersize);
    }
    vbo_->Add(0, totalbuffersize, (void*)triangles->data());
    vbo_->Bind();

    vao_->Add({0,3,sizeof(GLVertex),0});
    vao_->Add({1,3,sizeof(GLVertex), offsetof(GLVertex, normal_)});
    vao_->Add({2,2,sizeof(GLVertex), offsetof(GLVertex, texture_)});
    vao_->SetAttributes();

    Unbind();
}

void kipod::GLRenderLayout::SetupPointSet23(const std::vector<Vec5f>* vertices)
{
    LOG_ENGINE("Call: SetupPointSet23 from Vec5f data");
    unsigned long totalbuffersize = vertices->size()*sizeof(Vec5f);

    AddBufferData(GL_POINTS);

    vao_ = std::make_shared<kipod::VertexAttributeObject>();
    vao_->Set();

    if(OpenGLEngine::Version()==450){
        vbo_ = std::make_shared<kipod::VertexBuffer450>(nullptr, totalbuffersize);
        vbo_->count_ = vertices->size();
    }else{
        vbo_ = std::make_shared<kipod::VertexBuffer410>(nullptr, vertices->size(), totalbuffersize);
    }
    vbo_->Add(0, totalbuffersize, (void*)vertices->data());
    vbo_->Bind();

    vao_->Add({0, 2, sizeof(Vec5f), 0});
    vao_->Add({1, 3, sizeof(Vec5f), 2 * sizeof(GLfloat)});
    vao_->SetAttributes();

    Unbind();
}

void kipod::GLRenderLayout::SetupEdges23(const std::vector<Vec10f>* vertices)
{
    LOG_ENGINE("Call: SetupEdgesSet23 from Vec10f data");
    unsigned long totalbuffersize = vertices->size()*sizeof(Vec5f);

    AddBufferData(GL_POINTS);

    vao_ = std::make_shared<kipod::VertexAttributeObject>();
    vao_->Set();

    if(OpenGLEngine::Version()==450){
        vbo_ = std::make_shared<kipod::VertexBuffer450>(nullptr, totalbuffersize);
        vbo_->count_ = vertices->size();
    }else{
        vbo_ = std::make_shared<kipod::VertexBuffer410>(nullptr, vertices->size(), totalbuffersize);
    }
    vbo_->Add(0, totalbuffersize, (void*)vertices->data());
    vbo_->Bind();

    vao_->Add({0, 2, sizeof(Vec10f), 0});
    vao_->Add({1, 3, sizeof(Vec10f), 2 * sizeof(GLfloat)});
    vao_->Add({2, 2, sizeof(Vec10f), 5 * sizeof(GLfloat)});
    vao_->Add({3, 3, sizeof(Vec10f), 7 * sizeof(GLfloat)});
    vao_->SetAttributes();

    Unbind();
}
void kipod::GLRenderLayout::SetupShape(const std::vector<glm::vec2> *vertices)
{
    LOG_ENGINE("Call: Shape Setup");
    SetupLayout(*vertices, GL_TRIANGLE_FAN);

    for(const auto& v : *vertices)
        LOG_INFO("({},{})",v.x,v.y);
}

void kipod::GLRenderLayout::SetupPointSet(const std::vector<glm::vec4> *vertices)
{
    LOG_ENGINE("Call: PointSet Setup");
    SetupLayout(*vertices, GL_POINTS);
}

void kipod::GLRenderLayout::SetupLines(const std::vector<Vec3> *vertices, const std::vector<Vec3> *colors)
{
    LOG_ENGINE("Call: Lines Setup");
    SetupLayout(*vertices, *colors, GL_LINES);
}

void kipod::GLRenderLayout::SetupGrid(const std::vector<Vec3> *vertices)
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

unsigned int VectorLength(const glm::vec2&){ return 2;}
unsigned int VectorLength(const glm::vec3&){ return 3;}
unsigned int VectorLength(const glm::vec4&){ return 4;}
unsigned int VectorLength(const vec2&){ return 2;}
unsigned int VectorLength(const vec3&){ return 3;}
unsigned int VectorLength(const vec4&){ return 4;}

template<typename Vector, typename... MoreVectors>
void kipod::GLRenderLayout::AddBufferData(const std::vector<Vector>& vectors, MoreVectors... more_vectors)
{
    unsigned long buffersize = vectors.size()*sizeof(Vector);
    vbo_->count_ = vectors.size();

    vbo_->Add(buffersize, (void*)vectors.data());
    vao_->Add({vao_->NumberOfAttributes(), VectorLength(vectors[0]), sizeof(Vector),0});

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

