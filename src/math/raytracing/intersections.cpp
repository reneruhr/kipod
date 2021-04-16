#include "intersections.h"
namespace kipod{


double Discriminant(double a, double b, double c){
    return b*b-4*a*c;
}

std::pair<double,double>  roots2(double a, double b, double disc){
    double sqrt_disc = std::sqrt(disc);

    return { (-b - sqrt_disc)/(2*a), (-b + sqrt_disc)/(2*a) };
}

double  root(double a, double b){
    return -b/(2*a);
}

Intersections::Intersections(Ray* ray, RaytracingSphere* sphere)
{
    object_ = sphere;
   //Algebraic Method
    Vec3d o = sphere->center_;
    double r = sphere->radius_;
    Vec3d p0 = ray->Origin();
    Vec3d v = ray->Direction();

    Vec3d p1 = p0-o;

    double disc = Discriminant( Dot(v,v) , 2*Dot(p1,v) , Dot(p1,p1)-r*r );

    if(disc<0.0)
        return;
    else{
        auto[a,b] = roots2(Dot(v,v), 2*Dot(p1,v), disc );
        intersection_points_.push_back(a);
        intersection_points_.push_back(b);
    }
}
}
