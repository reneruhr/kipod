#pragma once
#include "ray.h"
#include <vector>
namespace kipod{
using Vec3d = siegel2021::Vector<double,3>;

class Intersections
{
    std::vector<double> intersection_points_;
    Ray* ray_;
    RaytracingObject* object_;
public:
    Intersections(Ray*, RaytracingObject*) {};
    Intersections(Ray* A, RaytracingSphere* B);

    int Count(){ return intersection_points_.size();}
    double Point(int i){
        assert(i<=Count());
        return intersection_points_[i-1];
    }

    bool operator()(){
        if(Count()) for(auto t : intersection_points_)  if(t>0) return true;
        return false;
    }

    Vec3d Point(){
            for(auto t : intersection_points_)  if(t>0) return ray_->At(t);            
    }
};

}
