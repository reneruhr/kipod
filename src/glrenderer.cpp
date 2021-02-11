#include "core.h"
#include "scene.h"
#include "meshmodel.h"
#include "glrenderer.h"
#include "render/initshader.h"
#include "pointset.h"

#include <numeric>
#include "utils/buffer_packing.h"

void GLRenderer::SwapPrograms()
{
  //  std::swap(programQuasiOctagon, programQuasiOctagonTexture);
}

void GLRenderer::drawTriangles(shared_ptr<ModelData> model)
{
    glBindVertexArray(model->vao);

    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
    glDrawElements(GL_TRIANGLES, model->indices_size, GL_UNSIGNED_INT, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GLRenderer::UpdatePoints(PointSet* points)
{
    shared_ptr<LatticeData> lattice = points->lattice_data_;
    lattice->size = size(points->sample_);

    glBindVertexArray(lattice->vao);
    glBindBuffer(GL_ARRAY_BUFFER, lattice->vbo);
    glBufferData(GL_ARRAY_BUFFER, lattice->size * 4 * sizeof(float), points->sample_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

shared_ptr<ShapeData> GLRenderer::UpdateShape(shared_ptr<ShapeData> shape, vector<vec2>* vertices_)
{
    glBindVertexArray(shape->vao_);
    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo_);
    glBufferData(GL_ARRAY_BUFFER, shape->size_ * sizeof(vec2), vertices_->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return shape;
}

void GLRenderer::SetupCoordinateAxis()
{
    coordinate_axis_ = std::make_unique<kipod::RenderObject>();

    std::vector<vec3> vertices = {vec3(-2,0,0),vec3(10,0,0),
                                  vec3(0,-2,0),vec3(0,10,0),
                                  vec3(0,0,-2),vec3(0,0,10),
                                 };
    std::vector<vec3> colors =   {vec3(1.0,0.5,0.5),vec3(1,0,0),
                                  vec3(0.5,1.0,0.5),vec3(0,1,0),
                                  vec3(0.5,0.5,1.0),vec3(0,0,1),
                                  };

    std::string name = "Coordinate Axis";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupLines(&vertices, &colors);
    layout->sha_ = new kipod::Shader("lines.vert.glsl", "lines.frag.glsl");
    layout->sha_->AttachUniform<glm::mat4>("mvp");
    coordinate_axis_->AddLayout(name, layout);
}

void GLRenderer::DrawCoordinateAxis(kipod::RenderCamera* camera){
    auto layout = static_cast<kipod::GLRenderLayout*>(coordinate_axis_->Layout("Coordinate Axis"));
    layout->sha_->Use();
    layout->sha_->SetUniform<glm::mat4>("mvp", *camera);
    layout->Draw();
    layout->sha_->Unuse();
}
