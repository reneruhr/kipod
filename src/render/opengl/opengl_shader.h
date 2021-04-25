#pragma once
#include "../../core.h"

struct  ShaderException : public std::exception {
    virtual const char* what () const throw (){
        return "Some Shader is not working. Quitting Kipod.";
    }
};

struct  ReadingShaderException : public ShaderException
{
    const char* what () const throw () {
        return "Shader source could not be read.";
    }
};

struct  EmptyShaderException : public ShaderException
{
    const char* what () const throw () {
        return "Shader source was read but empty.";
    }
};
struct  CompileShaderException : public ShaderException
{
    const char* what () const throw () {
        return "Shader was not compiled correctly.";
    }
};
struct  LinkingShaderException : public ShaderException
{
    const char* what () const throw () {
        return "Shaders were not linked correctly.";
    }
};

std::string* ReadShaderSource(std::filesystem::path path);
GLuint InitShader(const char* vShaderFile, const char* gShaderFile, const char* fShaderFile=nullptr, const char* cShaderFile = nullptr);


class OpenGLShader
{
public:
    OpenGLShader();
};


