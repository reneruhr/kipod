#include "../include/render_shader.h"
namespace kipod
{

//template <typename T>
//void Shader::SetUniform(std::string name, T &data)
//{
//    auto search = uniforms_.find(name);
//    if(search != end(uniforms_))
//        static_cast<Uniform<T>*>(search->second)->Set(data);
//    else
//        //AttachUniform<T>(name, data);
//    {
//        Uniform<T>* uniform = new Uniform<T>(name, program_);
//        uniform->Set(data);
//        uniforms_.insert({name, static_cast<IUniform*>(uniform)});
//    }
//}

//template <typename T>
//void Shader::AttachUniform(std::string name){
//    Uniform<T>* uniform = new Uniform<T>(name, program_);
//    uniforms_.insert({name, static_cast<IUniform*>(uniform)});
//}


}
