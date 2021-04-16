#include "shapes_polygon.h"

namespace kipod::Shapes{

Octagon::Octagon(float s): Polygon(s)
{
    const float nu = 1+sqrt(2);
    s*=0.5;
    float t = s*nu;
    vertices_.emplace_back(vec2{s,t});
    vertices_.emplace_back(vec2{-s,t});
    vertices_.emplace_back(vec2{-t,s});
    vertices_.emplace_back(vec2{-t,-s});
    vertices_.emplace_back(vec2{-s,-t});
    vertices_.emplace_back(vec2{s,-t});
    vertices_.emplace_back(vec2{t,-s});
    vertices_.emplace_back(vec2{t,s});

    UpdatedTransformedVertices();

    for(int i = 0; i<8; ++i)
        edges_.emplace_back(std::vector<vec2>{vertices_[i],vertices_[(i+1)%8]});
}

Square::Square(float s): Polygon(s)
{
    float t = s/2;
    vertices_.emplace_back(vec2{t,t});
    vertices_.emplace_back(vec2{-t,t});
    vertices_.emplace_back(vec2{-t,-t});
    vertices_.emplace_back(vec2{t,-t});

    UpdatedTransformedVertices();

    for(int i = 0; i<4; ++i)
        edges_.emplace_back(std::vector<vec2>{vertices_[i],vertices_[(i+1)%4]});
}


Rectangle::Rectangle(float w, float h): Polygon()
{
    float s = h/2;
    float t = w/2;
    vertices_.emplace_back(vec2{t,s});
    vertices_.emplace_back(vec2{-t,s});
    vertices_.emplace_back(vec2{-t,-s});
    vertices_.emplace_back(vec2{t,-s});

    UpdatedTransformedVertices();

    for(int i = 0; i<4; ++i)
        edges_.emplace_back(std::vector<vec2>{vertices_[i],vertices_[(i+1)%4]});
}



bool Polygon::IsInside(vec2 x)
{
    for(auto e : edges_) if(det(x-e[0],e[1]-e[0]) > 0) return false;
    return true;
}

void Polygon::UpdatedTransformedVertices()
{
    transformed_vertices_ = {};
    for(auto& v : vertices_)
        transformed_vertices_.emplace_back(transform_*v);
}

}
