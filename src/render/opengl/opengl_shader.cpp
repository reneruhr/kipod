#include "opengl_shader.h"
#include "../../core.h"

std::string* ReadShaderSource(std::filesystem::path path)
{
    std::ifstream t(path);
    if(t.fail()) throw ReadingShaderException();
    std::stringstream stream_buffer;
    stream_buffer << t.rdbuf();
    std::string* buffer = new std::string(stream_buffer.str());
    return buffer;
}

GLuint
InitShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    struct Shader {
        const char*  filename;
        GLenum       type;
        const GLchar*      source;
    } shaders[3] = {
        { vShaderFile, GL_VERTEX_SHADER, nullptr },
        { gShaderFile, GL_GEOMETRY_SHADER, nullptr },
        { fShaderFile, GL_FRAGMENT_SHADER, nullptr }
    };


    GLuint program = glCreateProgram();

    for ( int i = 0; i < 3; ++i ) {
        Shader& s = shaders[i];
        try {
            if(s.type==GL_GEOMETRY_SHADER && !s.filename) continue;
            s.source = ReadShaderSource( s.filename )->c_str();
        }  catch (ShaderException& e) {
            throw(e);
        }
        if (!shaders[i].source) {
            LOG_ENGINE("Failed to read shader {}", s.filename);
            throw EmptyShaderException();
        }

        GLuint shader = glCreateShader( s.type );
        glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );
        glCompileShader( shader );

        GLint  compiled;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            LOG_ENGINE("Failed to compile shader {}", s.filename);
            GLint  logSize;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
            char* logMsg = new char[logSize];
            glGetShaderInfoLog( shader, logSize, NULL, logMsg );
            LOG_ENGINE(logMsg);
            delete [] logMsg;
            delete [] s.source;
            throw CompileShaderException();
        }
        delete [] s.source;
        glAttachShader( program, shader );
    }
    glLinkProgram(program);
    GLint  linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked ) {
        LOG_ENGINE("Shader program could not link");
        GLint  logSize;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetProgramInfoLog( program, logSize, NULL, logMsg );
        LOG_ENGINE(logMsg);
        delete [] logMsg;
        throw LinkingShaderException();
    }
    return program;
}
