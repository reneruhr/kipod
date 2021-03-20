#include "softrenderer_light.h"

namespace kipod{

//float Light::attenuation(vec3 target)
//{
//    auto f = [this](float d){return this->attenuation_constant+this->attenuation_linear*d + this->attenuation_quadratic*d*d;};
//    return 1/f(dot(target=-vec3(&source.x), target ));
//}

vec4 colorAmbient(const vec4 &matColor, const vec4 &lightColor)
{
    return matColor*lightColor;
}

vec4 colorDiffuse(const vec4 &matColor, vec4 triangle[], vec4 normal_ends[], const mat4 &transform, RenderLight *light)
{
    vec4 loc = (triangle[0]+triangle[1]+triangle[2])/3.0;
    vec4 l = -1.0f*transform * light->Source()-loc;
    vec4 n = (normal_ends[0]+normal_ends[1]+normal_ends[2])/3.0;
    n.w = 0;
    n = normalize(n);
    vec4 lightColor = light->Color();

    return matColor*lightColor*std::max(dot(l,n),0.0f);
}

vec4 colorSpecular(const vec4 &matColor, const float &shininess, const vec4 &camLocation, vec4 triangle[], vec4 normal_ends[], const mat4 &transform, RenderLight *light)
{
    vec4 loc = (triangle[0]+triangle[1]+triangle[2])/3.0;
    vec4 l = -1.0f*transform* light->Source()+loc;
    vec4 n = (normal_ends[0]+normal_ends[1]+normal_ends[2])/3.0;
    n.w = 0;
    n = normalize(n);
    vec4 lightColor = light->Color();
    vec4 v = camLocation+loc;
    vec4 h = l+v;
    h.w =0;
    h = normalize(h);


    return matColor*lightColor*std::max(std::pow(dot(h,n), shininess), 0.0f);
}


}
