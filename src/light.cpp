#include "../include/light.h"

float Light::attenuation(vec3 target)
{
    auto f = [this](float d){return this->attenuation_constant+this->attenuation_linear*d + this->attenuation_quadratic*d*d;};
    return 1/f(dot(target=-vec3(&source.x), target ));
}
