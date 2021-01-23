#ifndef RENDERLIGHT_H
#define RENDERLIGHT_H

#include <glm/matrix.hpp>

#include "render_shader.h"
#include <string>

typedef enum LightSource{
    AMBIENT,
    POINT,
    DIFFUSE,
    SPECULAR,
    SPOTLIGHT,
    DISTANT
} LightSource;


namespace kipod
{

class RenderLight
{
protected:
    LightSource type_;
    glm::vec4 source_;
    glm::vec4 color_;
    int on_;
public:

    RenderLight(LightSource type, glm::vec4 source, glm::vec4 color):
        type_(type), source_(source), color_(color){}
    RenderLight():
        type_(LightSource::AMBIENT), source_(glm::vec4(0.0)), color_(glm::vec4(0.2,0.2,0.5,1)){}
    virtual ~RenderLight() = default;

    LightSource Type() const { return type_;}
    glm::vec4 Color()  const { return color_;}
    glm::vec4 Source() const { return source_;}
    int TurnedOn()     const { return on_;}
};

inline void AttachLightToShader(Shader& shader, int id)
{
    std::string lightsi = "Lights["+std::to_string(id)+"]";
    std::string type = lightsi+".type";
    std::string source = lightsi+".source";
    std::string color = lightsi+".color";
    std::string on = lightsi+".on";

    shader.AttachUniform<int>(type);
    shader.AttachUniform<glm::vec4>(source);
    shader.AttachUniform<glm::vec4>(color);
    shader.AttachUniform<int>(on);
}

inline void SetLightToShader(Shader& shader, int id, RenderLight *light)
{
    std::string lightsi = "Lights["+std::to_string(id)+"]";
    std::string type = lightsi+".type";
    std::string source = lightsi+".source";
    std::string color = lightsi+".color";
    std::string on = lightsi+".on";

    shader.SetUniform<int>(type, light->Type());
    shader.SetUniform<glm::vec4>(source, light->Source());
    shader.SetUniform<glm::vec4>(color, light->Color());
    shader.SetUniform<int>(on, light->TurnedOn());
}

} // kipod
#endif // RENDERLIGHT_H
