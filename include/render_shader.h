#ifndef SHADER_H
#define SHADER_H


#include <GL/glew.h>
#include "../include/initshader.h"
#include <string>
#include <unordered_map>
#include "render_uniform.h"

namespace kipod{

class Shader
{
    inline static std::string path = "shaders/";

    GLuint program_;
    std::unordered_map<std::string, IUniform*> uniforms_;

public:
    Shader() = default;

    Shader(std::string vert, std::string frag){
        program_ = Program(vert,frag);
    }

    void Use(){
        Shader::Use(program_);
    }

    static GLuint Program(std::string vert, std::string frag){

        GLuint program = InitShader((path+vert).c_str(), (path+frag).c_str() );
        LOG_ENGINE("Created Shader program {}.", program);
        return program;
    }

    static void Use(GLuint program)
    {
        //LOG_ENGINE("Use program {}.", program);
        glUseProgram(program);
    }

    static void Unuse()
    {
        glUseProgram(0);
    }

    template <typename T>
    void AttachUniform(std::string name){
        Uniform<T>* uniform = new Uniform<T>(name, program_);
        uniforms_.insert({name, static_cast<IUniform*>(uniform)});
    }

    template <typename T>
    void SetUniform(std::string name, const T& data){
        auto search = uniforms_.find(name);
        if(search != end(uniforms_))
            static_cast<Uniform<T>*>(search->second)->Set(data);
        else
        {
            LOG_ENGINE("Did not find uniform. Create a new one");
            Uniform<T>* uniform = new Uniform<T>(name, program_);
            uniform->Set(data);
            uniforms_.insert({name, static_cast<IUniform*>(uniform)});
        }
    }


};


}
#endif // SHADER_H
