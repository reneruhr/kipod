#pragma once
#include <utility>

#include "../../core.h"

namespace kipod::Shapes{
using Vec2  = glm::vec2;
using Mat2  = glm::mat2;

class Polygon {
protected:
    //Counter-Clockwise Order
    Vec2 center_ = {0,0};
    float side_length_;
    std::vector<Vec2> vertices_; // fixed
public:
    Polygon(){}
    explicit Polygon(float s) : side_length_(s) {}
    Polygon(std::vector<Vec2>  vertices, const Vec2& center = Vec2(0)) : center_(center), vertices_(std::move(vertices))
    {
        UpdatedTransformedVertices();

        for(int i = 0; i<vertices_.size(); ++i)
            edges_.emplace_back(std::make_pair(vertices_[i],vertices_[(i+1)%vertices_.size()]));
    }
    virtual ~Polygon() = default;

    Mat2 transform_{1.};

    std::vector<Vec2> transformed_vertices_; // = transform_ * vertices_
    std::vector< std::pair<Vec2,Vec2> > edges_;

    bool IsInside(const Vec2& x);
    void UpdatedTransformedVertices();
    int NumberEdges() const{ return edges_.size(); }
    int NumberVertices() const{ return transformed_vertices_.size(); }
    Vec2 GetVertex(int n) { return transformed_vertices_[n]; }
    Vec2 GetCenter() { return center_; }
    void SetCenter(const Vec2& c) { center_ = c; }
};

// Sides orthogonal to coordinate axes AKA truncated square.
// Centered at origin (0,0)
class Octagon : public Polygon
{
public:
    explicit Octagon(float s = 1);
};

class Square : public Polygon
{
public:
    explicit Square(float s = 1);
};

class Rectangle : public Polygon
{
public:
    explicit Rectangle(float w = 1, float h = 2);
};
}
