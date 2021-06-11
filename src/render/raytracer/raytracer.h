#pragma once
#include "../../core.h"
#include "../../kipod.h"
#include "raytracer_uniform.h"
#include "raytracer_layout.h"
#include "raytracer_framebuffer.h"
#include "../../math/raytracing/intersections.h"

namespace kipod{
class Light;
class MaterialStruct;
struct RaytracerBuffer;


class Raytracer
{
    std::unique_ptr<RaytracerFramebuffer> framebuffer_;
    std::unique_ptr<RaytracerUniform> uniform_;

    int width_, height_;

    void CreateBuffers();
    void CreateLocalBuffer();

    GLuint program;
    GLuint buffer;
    GLint  vPosition;
    GLint  vTexCoord;

    GLuint screen_texture_;
    GLuint screen_vertex_array_;
    void CreateOpenGLBuffer();
    void InitOpenGLRendering();


public:
    Raytracer(int width, int height);
    ~Raytracer() {};

    void SetUniforms(RenderCamera* camera, mat4 transform);


    void DrawToOpenGL();
    void ClearColorBuffer();
    void ClearBuffer();

    void DrawPoint(int x, int y, Vec3f* color);
};

}
