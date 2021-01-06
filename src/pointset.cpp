#include "../include/pointset.h"


void PointSet::Init(GLRenderer *glrenderer, WindowType type)
{
    LOG("Init PointSet");
    MakeSample();
    lattice_data_ = glrenderer->loadPoints(this);
    lattice_data_->qc.window = type;
    glrenderer->initUniformBlock(lattice_data_);
}

void PointSet::Draw(GLRenderer *glrenderer)
{
    glrenderer->drawPoints(lattice_data_);
}
