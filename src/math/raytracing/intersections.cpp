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

Intersections::Intersections(Ray *ray, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{ //https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    glm::vec3 edge1, edge2, h, s, q;
    float e,f,u,v;
    edge1 = b - a;
    edge2 = c - a;
    h = glm::cross(ray->Direction(), edge2);
    e = glm::dot(edge1, h);
    if (e > -EPS_HIT_MIN && e < EPS_HIT_MIN)
        return;    // This ray is parallel to this triangle.
    f = 1.0/e;
    s = ray->Origin() - a;
    u = f * glm::dot(s, h);
    if (u < 0.0 || u > 1.0)
        return;
    q = glm::cross(s, edge1);
    v = f * glm::dot(ray->Direction(), q);
    if (v < 0.0 || u + v > 1.0)
        return;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * glm::dot(edge2, q);
    if (t > EPS_HIT_MIN && t < EPS_HIT_MAX) // ray intersection
    {
        intersection_points_.push_back(t);
        return;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return;

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
