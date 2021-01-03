#include "../include/pointset.h"


void PointSet::init(GLRenderer *glrenderer)
{
    LOG("Init PointSet");
    makeSample3d(sampleInfo);
    latticeData = glrenderer->loadPoints(this);
}

void PointSet::draw(GLRenderer *glrenderer)
{
    glrenderer->drawPoints(latticeData);
}
