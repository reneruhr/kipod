#pragma once
#include "GL/glew.h"
char* readShaderSource(const char* shaderFile);

GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

GLuint InitShader(const char* vShaderFile, const char* gShaderFile, const char* fShaderFile);
