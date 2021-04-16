#include "raytracer.h"
#include "raytracer_layout.h"
namespace kipod{


Raytracer::Raytracer(int width, int height) :    
    framebuffer_(std::make_unique<RaytracerFramebuffer>(width, height)),
    uniform_(std::make_unique<RaytracerUniform>()),
    width_(width),height_(height)
{
    InitOpenGLRendering();
    CreateBuffers();
}

void Raytracer::SetUniforms(RenderCamera *camera, mat4 transform)
{
    uniform_->camera_ = camera;
    uniform_->object_transform_ = transform;
}


void Raytracer::CreateBuffers()
{
    CreateOpenGLBuffer();
}


void Raytracer::ClearBuffer()
{
    framebuffer_->Clear();
}

void Raytracer::DrawPoint(int x, int y, Vec3f *color)
{
    framebuffer_->DrawPoint(x,y,color);
}


void Raytracer::InitOpenGLRendering()
{
    glGenTextures(1, &gScreenTex);
    glGenVertexArrays(1, &gScreenVtc);
    GLuint buffer;
    glBindVertexArray(gScreenVtc);
    glGenBuffers(1, &buffer);
    const GLfloat vtc[]={
        -1, -1,
        1, -1,
        -1, 1,
        -1, 1,
        1, -1,
        1, 1
    };
    const GLfloat tex[]={
        0,0,
        1,0,
        0,1,
        0,1,
        1,0,
        1,1};
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

    program = InitShader( "shaders/softrenderer.vert.glsl", "shaders/softrenderer.frag.glsl" );
    glUseProgram( program );
    GLint  vPosition = glGetAttribLocation( program, "vPosition" );

    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vtc) );
    glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );
}

void Raytracer::CreateOpenGLBuffer()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gScreenTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width_, height_, 0, GL_RGB, GL_FLOAT, NULL);
    glViewport(0, 0, width_, height_);
}

void Raytracer::DrawToOpenGL()
{
    glUseProgram( program );

    glBindVertexArray(gScreenVtc);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gScreenTex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGB, GL_FLOAT, framebuffer_->Data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}





}
