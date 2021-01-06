#ifndef POLYGON_H
#define POLYGON_H

#include "../utils/vec.h"
#include <vector>

class Polygon
{
public:
    Polygon();

    //Counter-Clockwise Order
    std::vector< std::vector<vec2> > edges_;

    bool Inside(vec2 x){
        for(auto e : edges_) if(det(x-e[0],e[1]-e[0])<0) return false;
        return true;
    }
};


const float nu = 1+sqrt(2);
// Sides orthogonal to coordinate axes AKA truncated square.
class Octagon : public Polygon
{
public:
    Octagon(float s = 1): Polygon(), side_length_(s){
        float t = s/2*nu;
        vertices_.emplace_back(vec2{s,t});
        vertices_.emplace_back(vec2{-s,t});
        vertices_.emplace_back(vec2{-t,s});
        vertices_.emplace_back(vec2{-t,-s});
        vertices_.emplace_back(vec2{-s,-t});
        vertices_.emplace_back(vec2{s,-t});
        vertices_.emplace_back(vec2{t,-s});
        vertices_.emplace_back(vec2{t,s});

        for(int i = 0; i<8; ++i) edges_.emplace_back(std::vector<vec2>{vertices_[i],vertices_[(i+1)%8]});
    }
    float side_length_;
    std::vector<vec2> vertices_;

};


#endif // POLYGON_H
