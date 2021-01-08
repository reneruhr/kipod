#ifndef LIGHT_H
#define LIGHT_H

#include "core.h"

typedef struct MaterialStruct {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emission;
    float shininess;

    MaterialStruct( vec4 ambient,vec4 diffuse,vec4 specular,vec4 emission,float shininess):
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        emission(emission),
        shininess(shininess){}

    MaterialStruct(float s):
        ambient(s,s,s,1),
        diffuse(s,s,s,1),
        specular(s,s,s,1),
        emission(s,s,s,1),
        shininess(1.0f){}

    MaterialStruct():
        ambient(0.1,0.1,0.6,1),
        diffuse(0.7,0.3,0.2,1),
        specular(0.4,0.4,0.4,1),
        emission(0.0,0.0,0.0,1),
        shininess(1.0f){}

} MaterialStruct;


typedef enum LightSource{
    AMBIENT,
    POINT,
    DIFFUSE,
    SPECULAR,
    SPOTLIGHT,
    DISTANT
} LightSource;

class Light
{
    LightSource type;
    vec4 source;
    vec4 color;

    //Point Data
    float attenuation_constant,
          attenuation_linear,
          attenuation_quadratic;
    // SpotLight Data
    float spotlightAngle;
    friend class GUI;
public:
    Light(LightSource type, vec4 source, vec4 color): type(type), source(source), color(color){}
    Light():type(LightSource::AMBIENT), source(vec4(0.0f)), color(vec4(0.2,0.2,0.5,1)){}

    LightSource getType(){ return type;}
    vec4 getColor(){ return color;}
    vec4 getSource(){ return source;}
    float attenuation(vec3 target);

    bool turnedOn = true;
};

#endif // LIGHT_H
