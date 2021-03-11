#pragma once

#include <GL/glew.h>
#include "opengl/opengl_shader.h"
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
    Shader(std::string vert, std::string frag, std::string geom = {})
    {
        try {
            program_ = Program(vert, frag, geom);
        }  catch (ShaderException& e) {
           exit(EXIT_FAILURE);
        }
    }

    operator GLuint()  const {
        return program_;
    }

    void Use()
    {
        Shader::Use(program_);
    }

    static GLuint Program(std::string vert, std::string frag, std::string geom={})
    {
        GLuint program;
        try {
            program = InitShader((path+vert).c_str(),
                                        (path+frag).c_str(),
                                        geom.empty() ? nullptr : (path+geom).c_str()
                                        );
        }  catch (ShaderException& e) {
            std::cout << e.what();
            throw(e);
        }
        LOG_ENGINE("Created Shader program {}.", program);
        return program;
    }

    static void Use(GLuint program)
    {
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
    void SetUniform(std::string name, const T& data, const int size = 1){
        auto search = uniforms_.find(name);
        if(search != end(uniforms_))
            static_cast<Uniform<T>*>(search->second)->Set(data, size);
        else
        {
            LOG_ENGINE("Did not find uniform. Create a new one");
            Uniform<T>* uniform = new Uniform<T>(name, program_);
            uniform->Set(data, size);
            uniforms_.insert({name, static_cast<IUniform*>(uniform)});
        }
    }


};


}
