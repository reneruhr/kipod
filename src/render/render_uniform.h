#pragma once
#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

#include "../utils/log.h"

namespace kipod{

class IUniform{
public:
    std::string name_;

    IUniform() = default;
    IUniform(std::string& name) : name_(name){}

    virtual void Name() { LOG_ENGINE("Uniform name: {}", name_);}
    virtual ~IUniform() = default;

};


template <typename T>
class Uniform :  public IUniform
{
public:
    T data_;

    GLuint program_;
    GLint location_;
    Uniform(std::string name_, GLuint program) : IUniform(name_), program_(program){
        location_ = GetLocation();
        LOG_ENGINE("Created New Uniform {} to program {} at location {}", name_, program, location_);
    }
    GLint GetLocation(){
        return  glGetUniformLocation(program_, name_.c_str());
    }

    void Set(const T& data, const int size = 1)
    {
        data_ = data;
        if constexpr (std::is_same_v<T, glm::mat4>)
            glUniformMatrix4fv(location_, size, GL_FALSE, glm::value_ptr(data_));
        else if constexpr (std::is_same_v<T, glm::vec4>)
            glUniform4fv(location_, size, glm::value_ptr(data_));
        else if constexpr (std::is_same_v<T, glm::vec3>)
            glUniform3fv(location_, size, glm::value_ptr(data_));
        else if constexpr (std::is_same_v<T, glm::vec2>)
            glUniform2fv(location_, size, glm::value_ptr(data_));
        else if constexpr (std::is_same_v<T, float>)
            glUniform1f(location_, data_);
        else if constexpr (std::is_same_v<T, int>)
            glUniform1i(location_, data_);

//        else if constexpr (std::is_same_v<T, std::vector<glm::vec2> >)
//            glUniform2fv(location_, size, &data_[0][0]);
    }
};


}
