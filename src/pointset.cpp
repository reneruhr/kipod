#include "../include/pointset.h"


void PointSet::Init(GLRenderer *glrenderer)
{
    LOG("Init PointSet");
    MakeSample3d(sample_info_);
    lattice_data_ = glrenderer->loadPoints(this);
    glrenderer->initUniformBlock(lattice_data_);
}

void PointSet::Draw(GLRenderer *glrenderer)
{
    glrenderer->drawPoints(lattice_data_);
}
