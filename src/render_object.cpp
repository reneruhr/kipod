#include "../include/render_object.h"
#include "../include/utils/buffer_packing.h"



void kipod::GLRenderLayout::Draw()
{
    //glEnable(GL_DEPTH_TEST);

//    if(sha_){
//        sha_->Use();
//    }

    if(tex_){
        glActiveTexture(GL_TEXTURE0);
        tex_->Bind();
    }

    vao_->Bind();
    ebo_->Bind();
    glDrawElements(ebo_->primitive_, ebo_->count_, GL_UNSIGNED_INT, (void*)0);
    vao_->Unbind();
    ebo_->Unbind();

//    if(sha_){
//        sha_->Unuse();
//    }

    //glDisable(GL_DEPTH_TEST);
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


void kipod::RenderObject::Draw(kipod::RenderLayout *layout)
{
    if(layout)  layout->Draw();
}

void kipod::RenderObject::Setup(kipod::RenderLayout *layout)
{
    if(layout)  layout->Setup();
}


void kipod::GLRenderLayout::SetupColoredTriangles(const std::vector<vec3>* vertices, const std::vector<unsigned int>* indices,
                                                  const std::vector<vec3>* normals,  const std::vector<unsigned int>* nindices)
{
    LOG_ENGINE("Call: Colored Triangles Setup");
    std::vector<vec3> vnVector;
    std::vector<unsigned int> indices_vector;
    pack_vectors(*vertices, *normals, vnVector, *indices, *nindices, indices_vector);

    ebo_ = new kipod::ElementsBuffer((void*)indices_vector.data(), indices_vector.size(), indices_vector.size()*sizeof(unsigned int));
    ebo_->Set();

    vao_ = new kipod::VertexAttributeObject;
    vao_->Set();

    unsigned int buffersize = vnVector.size()*sizeof(vec3);
    vbo_ = new kipod::VertexBuffer(nullptr, buffersize);
    vbo_->Add(0, buffersize, (void*)vnVector.data());

    vbo_->Bind();

    kipod::Attribute* att_v = new kipod::Attribute(0,3,2*sizeof(vec3),0);
    kipod::Attribute* att_n = new kipod::Attribute(1,3,2*sizeof(vec3), sizeof(vec3));

    vao_->Add(att_v);
    vao_->Add(att_n);
    vao_->SetAttributes();

    Unbind();
}


void kipod::GLRenderLayout::SetupColoredTriangles(const std::vector<vec3> *vertices, const std::vector<unsigned int> *indices_vector)
{
    LOG_ENGINE("Call: Colored Triangles Without Normals Setup");
    ebo_ = new kipod::ElementsBuffer((void*)indices_vector->data(), indices_vector->size(), indices_vector->size()*sizeof(unsigned int));
    ebo_->Set();

    vao_ = new kipod::VertexAttributeObject;
    vao_->Set();

    unsigned int buffersize = vertices->size()*sizeof(vec3);
    vbo_ = new kipod::VertexBuffer(nullptr, buffersize);
    vbo_->Add(0, buffersize, (void*)vertices->data());

    vbo_->Bind();

    kipod::Attribute* att_v = new kipod::Attribute(0,3,sizeof(vec3),0);

    vao_->Add(att_v);
    vao_->SetAttributes();

    Unbind();
}








void kipod::GLRenderLayout::SetupGLTriangles(const std::vector<GLTriangle>* triangles, const std::vector<unsigned int>* indices)
{
    LOG_ENGINE("Call: Textured Triangles Setup");

    ebo_ = new kipod::ElementsBuffer((void*)indices->data(), indices->size(), indices->size()*sizeof(unsigned int));
    ebo_->Set();

    vao_ = new kipod::VertexAttributeObject;
    vao_->Set();

    unsigned int buffersize = triangles->size()*sizeof(GLTriangle);
    vbo_ = new kipod::VertexBuffer(nullptr, buffersize);
    vbo_->Add(0, buffersize, (void*)triangles->data());

    vbo_->Bind();

    kipod::Attribute* att_v = new kipod::Attribute(0,3,sizeof(GLVertex),0);
    kipod::Attribute* att_n = new kipod::Attribute(1,3,sizeof(GLVertex), offsetof(GLVertex, normal_));
    kipod::Attribute* att_t = new kipod::Attribute(2,2,sizeof(GLVertex), offsetof(GLVertex, texture_));

    vao_->Add(att_v);
    vao_->Add(att_n);
    vao_->Add(att_t);
    vao_->SetAttributes();

    Unbind();
}







