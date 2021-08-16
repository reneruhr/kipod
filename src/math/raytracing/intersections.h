#pragma once
#include "ray.h"
#include <vector>

namespace kipod{

class Intersections
{
    std::vector<float> intersection_points_;
    Ray* ray_;
    RaytracingObject* object_;
public:
    Intersections(Ray*, RaytracingObject*) {}
    Intersections(Ray* A, RaytracingSphere* B);
    Intersections(Ray* A, RaytracingQuadric* B);
    Intersections(Ray* A, glm::vec3 a, glm::vec3 b, glm::vec3 c); //Triangle

    int Count(){ return intersection_points_.size(); }

    float Point(int i);

    bool operator()();

    glm::vec3 Point();
};

}
