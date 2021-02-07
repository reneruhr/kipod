#pragma once
#include <glm/matrix.hpp>
#include "render_shader.h"

namespace kipod {

class RenderMaterial
{
public:

    glm::vec4 ambient_;
    glm::vec4 diffuse_;
    glm::vec4 specular_;
    glm::vec4 emission_;
    float shininess_;

    RenderMaterial( glm::vec4 ambient,
                    glm::vec4 diffuse,
                    glm::vec4 specular,
                    glm::vec4 emission,
                    float shininess):
        ambient_(ambient),
        diffuse_(diffuse),
        specular_(specular),
        emission_(emission),
        shininess_(shininess){}

    RenderMaterial(float s):
        ambient_(s,s,s,1),
        diffuse_(s,s,s,1),
        specular_(s,s,s,1),
        emission_(s,s,s,1),
        shininess_(1.0f){}

    RenderMaterial():
        ambient_(0.1,0.1,0.6,1),
        diffuse_(0.7,0.3,0.2,1),
        specular_(0.4,0.4,0.4,1),
        emission_(1.0,0.0,0.0,1),
        shininess_(1.0f){}

};

inline void AttachMaterialToShader(Shader& shader)
{
    shader.AttachUniform<glm::vec4>("material.ambient");
    shader.AttachUniform<glm::vec4>("material.diffuse");
    shader.AttachUniform<glm::vec4>("material.specular");
    shader.AttachUniform<glm::vec4>("material.emission");
    shader.AttachUniform<float>("material.shininess");
}

inline void SetMaterialToShader(Shader& shader, const RenderMaterial &material)
{
    shader.SetUniform<glm::vec4>("material.ambient", material.ambient_);
    shader.SetUniform<glm::vec4>("material.diffuse", material.diffuse_);
    shader.SetUniform<glm::vec4>("material.specular",material.specular_);
    shader.SetUniform<glm::vec4>("material.emission",material.emission_);
    shader.SetUniform<float>("material.shininess",   material.shininess_);
}

} // kipod
