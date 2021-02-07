#include "render_uniform.h"

#include <type_traits>

namespace kipod{

//template <typename T>
//void Uniform<T>::Set(T& data)
//{
//    data_ = data;
//    if constexpr (std::is_same_v<T, glm::mat4>)
//        glUniformMatrix4fv(location_, 1, GL_FALSE, &data_[0][0]);
//    else if constexpr (std::is_same_v<T, glm::vec4>)
//        glUniform4fv(location_, 1, &data_[0]);
//    else if constexpr (std::is_same_v<T, glm::vec3>)
//        glUniform3fv(location_, 1, &data_[0]);
//    else if constexpr (std::is_same_v<T, glm::vec2>)
//        glUniform2fv(location_, 1, &data_[0]);
//    else if constexpr (std::is_same_v<T, float>)
//        glUniform1f(location_, data_);
//    else if constexpr (std::is_same_v<T, int>)
//        glUniform1i(location_, data_);
//}

}
