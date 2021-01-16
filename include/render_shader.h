#ifndef SHADER_H
#define SHADER_H


#include <GL/glew.h>
#include "../include/initshader.h"
#include <string>
#include <unordered_map>
#include "render_uniform.h"

class Shader
{
    inline static std::string path = "shaders/";

    GLuint program;
    std::unordered_map<std::string, IUniform*> uniforms_;

public:
    Shader();

    Shader(std::string vert, std::string frag){
        program = Program(vert,frag);
    }

    static GLuint Program(std::string vert, std::string frag){
        return InitShader((path+vert).c_str(), (path+frag).c_str() );
    }

    static void Use(GLuint program)
    {
        glUseProgram(program);
    }

//    template <typename T>
//    void AttachUniform(std::string name, T& data){
//        Uniform<T>* uniform = new Uniform<T>(name, program);
//        uniform->Set(data);
//        uniforms_.insert(name, uniform);
//    }

//    template <typename T>
//    void AttachUniform(std::string name){
//        Uniform<T>* uniform = new Uniform<T>(name, program);
//        uniforms_.insert(name, uniform);
//    }

    template <typename T>
    void SetUniform(std::string name, T& data){
        auto search = uniforms_.find(name);
        if(search != end(uniforms_))
            ((Uniform<T>*)search->second)>Set(data);
        else
            //AttachUniform<T>(name, data);
        {
            Uniform<T>* uniform = new Uniform<T>(name, program);
            uniform->Set(data);
            uniforms_.insert(name, uniform);
        }
    }


};

#endif // SHADER_H
