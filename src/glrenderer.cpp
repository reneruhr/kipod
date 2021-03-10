#include "core.h"
//#include "scene.h"
#include "modules/meshmodels/meshmodel.h"
#include "glrenderer.h"
#include "render/opengl/opengl_shader.h"
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


