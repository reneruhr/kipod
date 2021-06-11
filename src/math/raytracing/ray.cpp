#include "ray.h"
namespace kipod{

std::ostream& operator<<(std::ostream& os,  Ray& ray)
{
    os << "Ray Origin: (" << ray.Origin().x << ", " << ray.Origin().y << ", " << ray.Origin().z << ")\n";
    os << "Ray Direction: (" << ray.Direction().x << ", " << ray.Direction().y << ", " << ray.Direction().z << ")\n";

    return os;
}

}
