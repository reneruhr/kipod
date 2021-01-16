#ifndef RENDER_UNIFORM_H
#define RENDER_UNIFORM_H
#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <vector>
#include <string>


class IUniform{
protected:
    std::string name_;

    IUniform(std::string& name) : name_(name){};

};


template <typename T>
class Uniform : public IUniform
{
    T data_;

    GLuint program_;
    GLint location_;
    Uniform(std::string name_, GLuint program) : IUniform(name_), program_(program){
        location_ = GetLocation();
    }
public:
    GLint GetLocation(){
        return  glGetUniformLocation(program_, name_.c_str());
    }
    virtual void Set(T& data);

    virtual void SetMatrix4fv(glm::mat4& m);
    virtual void Set4fv(glm::vec4& v);
    virtual void Set3fv(glm::vec3& v);
    virtual void Set2fv(glm::vec2& v);
    virtual void Set1f(float& f);
    virtual void Set1i(int& i);
};

#endif // RENDER_UNIFORM_H
