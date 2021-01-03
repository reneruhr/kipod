#ifndef QUACRY_H
#define QUACRY_H

#include "utils/mat.h"

class Scene;
class PointSet;
class PrimMeshModel;


class QuaCry
{
    Scene* scene_;
    PointSet* pointset_;
    PrimMeshModel* window_box_;
    mat4 window_size_;
public:
    QuaCry(Scene* scene);
};

#endif // QUACRY_H
