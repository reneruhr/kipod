#pragma once
#include "../render_light.h"


namespace kipod{

//typedef struct MaterialStruct {
//    vec4 ambient;
//    vec4 diffuse;
//    vec4 specular;
//    vec4 emission;
//    float shininess;

//    MaterialStruct( vec4 ambient,vec4 diffuse,vec4 specular,vec4 emission,float shininess):
//        ambient(ambient),
//        diffuse(diffuse),
//        specular(specular),
//        emission(emission),
//        shininess(shininess){}

//    MaterialStruct(float s):
//        ambient(s,s,s,1),
//        diffuse(s,s,s,1),
//        specular(s,s,s,1),
//        emission(s,s,s,1),
//        shininess(1.0f){}

//    MaterialStruct():
//        ambient(0.1,0.1,0.6,1),
//        diffuse(0.7,0.3,0.2,1),
//        specular(0.4,0.4,0.4,1),
//        emission(0.0,0.0,0.0,1),
//        shininess(1.0f){}

//} MaterialStruct;

//class Light : public kipod::RenderLight
//{
//    kipod::LightSource type;
//    vec4 source;
//    vec4 color;

//    //Point Data
//    float attenuation_constant,
//          attenuation_linear,
//          attenuation_quadratic;
//    // SpotLight Data
//    float spotlightAngle;
//public:
//    Light(kipod::LightSource type, vec4 source, vec4 color):
//        RenderLight(type, MakeGLM(source), MakeGLM(color)),
//        type(type), source(source), color(color){}
//    Light():
//        RenderLight(),
//        type(kipod::LightSource::AMBIENT), source(vec4(0.0f)), color(vec4(0.2,0.2,0.5,1)){}

//    kipod::LightSource getType(){ return type;}
//    vec4 getColor(){ return color;}
//    vec4 getSource(){ return source;}
//    float attenuation(vec3 target);

//    bool turnedOn = true;
//};

vec4 colorAmbient(const vec4& matColor, const vec4& lightColor);

vec4 colorDiffuse(const vec4& matColor,
                  vec4 triangle[3], vec4 normal_ends[3],
                  const mat4& transform,
                  RenderLight* light);

vec4 colorSpecular(const vec4& matColor,
                   const float& shininess,
                   const vec4& camLocation,
                   vec4 triangle[3],
                   vec4 normal_ends[3],
                   const mat4& transform,
                   RenderLight* light);

}
