#include "intersections.h"

#define EPS_HIT_MIN 0.000001
#define EPS_HIT_MAX 1.e+07

namespace kipod{


float Discriminant(float a, float b, float c)
{
    return b*b-4*a*c;
}

std::pair<float,float>  roots2(float a, float b, float disc)
{
    float sqrt_disc = std::sqrt(disc);

    return { (-b - sqrt_disc)/(2*a), (-b + sqrt_disc)/(2*a) };
}

Intersections::Intersections(Ray* ray, RaytracingSphere* sphere)
{
    object_ = sphere;
   //Algebraic Method
    glm::vec3 o = sphere->center_;
    float r = sphere->radius_;
    glm::vec3 p0 = ray->Origin();
    glm::vec3 v = ray->Direction();

    glm::vec3 ray_to_sphere = p0-o;

    float disc = Discriminant( glm::dot(v,v) , 2*glm::dot(ray_to_sphere,v) , glm::dot(ray_to_sphere,ray_to_sphere)-r*r );

    if(disc<0)
        return;
    else{
        auto [a,b] = roots2(glm::dot(v,v), 2*glm::dot(ray_to_sphere,v), disc );
        intersection_points_.push_back(a);
        intersection_points_.push_back(b);
    }
}

Intersections::Intersections(Ray *ray, RaytracingQuadric *quadric)
{            // https://people.cs.clemson.edu/~dhouse/courses/405/notes/quadrics.pdf
            object_ = quadric;

            glm::vec4 p = glm::vec4(ray->Origin(),   1);
            glm::vec4 u = glm::vec4(ray->Direction(),0);

            glm::vec4 Qp = quadric->Q_transformed_*p;
            glm::vec4 Qu = quadric->Q_transformed_*u;

            float pQp = dot(p,Qp);
            float uQu = dot(u,Qu);
            float uQp = dot(u,Qp);


            float disc = uQp*uQp - uQu * pQp;
            float denom = 1/uQu;
            float sqrt_disc = sqrt(disc);
            if (disc > 0.0){
                float a = (-uQp - sqrt_disc)*denom;
                float b = (-uQp + sqrt_disc)*denom;
                intersection_points_.push_back(a);
                intersection_points_.push_back(b);
            }
}

float Intersections::Point(int i)
{
    assert(i<=Count());
    return intersection_points_[i-1];
}

bool Intersections::operator()()
{
    if(Count()) for(auto t : intersection_points_)  if((t > EPS_HIT_MIN) && (t < EPS_HIT_MAX)) return true;
    return false;
}

glm::vec3 Intersections::Point()
{
    for(auto t : intersection_points_)  if((t > EPS_HIT_MIN) && (t < EPS_HIT_MAX)) return ray_->At(t);
    return glm::vec3(0,0,0); // Dummy return value
}


}
