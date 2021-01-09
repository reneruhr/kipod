#include "../include/pointset.h"


void PointSet::Init(GLRenderer *glrenderer)
{
    LOG("Init PointSet");
    MakeSample();
    lattice_data_ = glrenderer->loadPoints(this);
}


void PointSet::Draw(GLRenderer *glrenderer)
{
    glrenderer->drawPoints(lattice_data_);
}
