#include "../include/render_uniform.h"

#include <type_traits>

template <typename T>
void Uniform<T>::Set(T& data)
{
    data_ = data;
    if constexpr (std::is_same_v<T, glm::mat4>)
        glUniformMatrix4fv(location_, 1, GL_FALSE, &data_[0][0]);
    else if constexpr (std::is_same_v<T, glm::vec4>)
        glUniform4fv(location_, 1, &data_[0]);
    else if constexpr (std::is_same_v<T, glm::vec3>)
        glUniform3fv(location_, 1, &data_[0]);
    else if constexpr (std::is_same_v<T, glm::vec2>)
        glUniform2fv(location_, 1, &data_[0]);
    else if constexpr (std::is_same_v<T, float>)
        glUniform1f(location_, data_[0]);
    else if constexpr (std::is_same_v<T, int>)
        glUniform41i(location_, data_[0]);
}

template <typename T>
void Uniform<T>::Set4fv(glm::vec4& v)
{
    data_= v;
    glUniform4fv(location_, 1, &v[0]);
}

template <typename T>
void Uniform<T>::Set3fv(glm::vec3& v)
{
    data_= v;
    glUniform3fv(location_, 1, &v[0]);
}

template <typename T>
void Uniform<T>::Set2fv(glm::vec2& v)
{
    data_= v;
    glUniform2fv(location_, 1, &v[0]);
}

template <typename T>
void Uniform<T>::Set1f(float& f)
{
    data_= f;
    glUniform1f(location_, f);
}

template <typename T>
void Uniform<T>::Set1i(int& i)
{
    data_= i;
    glUniform1i(location_, i);
}
