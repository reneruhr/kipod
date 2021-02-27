#pragma once

#include "../../core.h"

namespace kipod::Shapes{


class Polygon
{
protected:
    //Counter-Clockwise Order
    vec2 center_ = {0,0};
    float side_length_;
    std::vector<vec2> vertices_; // fixed
public:
    Polygon(){}
    Polygon(float s) : side_length_(s) {}
    virtual ~Polygon() = default;

    mat2 transform_;

    std::vector<vec2> transformed_vertices_; // = transform_ * vertices_
    std::vector< std::vector<vec2> > edges_;

    bool IsInside(vec2 x);
    void UpdatedTransformedVertices();
};


// Sides orthogonal to coordinate axes AKA truncated square.
// Centered at origin (0,0)
class Octagon : public Polygon
{
public:
    Octagon(float s = 1);
};

class Square : public Polygon
{
public:
    Square(float s = 1);
};

}
