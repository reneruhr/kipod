#include "../include/render_object.h"

void kipod::GLObject::Draw()
{
    glEnable(GL_DEPTH_TEST);

    if(sha_){
        sha_->Use();
    }

    if(tex_){
        glActiveTexture(GL_TEXTURE0);
        tex_->Bind();
    }

    vao_->Bind();
    ebo_->Bind();
    glDrawElements(GL_TRIANGLES, ebo_->count_, GL_UNSIGNED_INT, (void*)0);
    vao_->Unbind();
    ebo_->Unbind();

    if(sha_){
        sha_->Unuse();
    }

    glDisable(GL_DEPTH_TEST);
}

void kipod::GLObject::Setup()
{
    ebo_->Set();
    vao_->Set();
    vbo_->Bind();
    vao_->SetAttributes();
    ebo_->Unbind();
    vbo_->Unbind();
    vao_->Unbind();
}
