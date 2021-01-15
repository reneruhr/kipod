#include "../include/render_object.h"

template<> void Uniform<glm::mat4>::Set(glm::mat4& m)
{
    data_= m;
    glUniformMatrix4fv(location_, 1, GL_FALSE, &data_[0][0]);
}

template<> void Uniform<glm::vec4>::Set(glm::vec4& v)
{
    data_= v;
    glUniform4fv(location_, 1, &v[0]);
}

template<> void Uniform<glm::vec3>::Set(glm::vec3& v)
{
    data_= v;
    glUniform2fv(location_, 1, &v[0]);
}

template<> void Uniform<glm::vec2>::Set(glm::vec2& v)
{
    data_= v;
    glUniform2fv(location_, 1, &v[0]);
}

template<> void Uniform<float>::Set(float& f)
{
    data_= f;
    glUniform1f(location_, f);
}

template<> void Uniform<int>::Set(int& i)
{
    data_= i;
    glUniform1f(location_, i);
}

