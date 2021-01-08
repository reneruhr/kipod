#ifndef POLYGON_H
#define POLYGON_H

#include "../utils/vec.h"
#include <vector>

class Polygon
{
public:
    Polygon(){}
    Polygon(float s) : side_length_(s) {}

    //Counter-Clockwise Order
    vec2 center_ = {0,0};
    float side_length_;
    std::vector<vec2> vertices_;
    std::vector< std::vector<vec2> > edges_;

    bool Inside(vec2 x){
        for(auto e : edges_) if(det(x-e[0],e[1]-e[0])<0) return false;
        return true;
    }
};


const float nu = 1+sqrt(2);
// Sides orthogonal to coordinate axes AKA truncated square.
// Centered at origin (0,0)
class Octagon : public Polygon
{

public:
    Octagon(float s = 1): Polygon(s)
    {
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

        for(int i = 0; i<8; ++i)
            edges_.emplace_back(std::vector<vec2>{vertices_[i],vertices_[(i+1)%8]});
    }
};

class Square : public Polygon
{

public:
    Square(float s = 1): Polygon(s)
    {
        float t = s/2;
        vertices_.emplace_back(vec2{t,t});
        vertices_.emplace_back(vec2{-t,t});
        vertices_.emplace_back(vec2{-t,-t});
        vertices_.emplace_back(vec2{t,-t});

        for(int i = 0; i<4; ++i)
            edges_.emplace_back(std::vector<vec2>{vertices_[i],vertices_[(i+1)%4]});
    }
};


#endif // POLYGON_H
