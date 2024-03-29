#pragma once
#include "../../core.h"
#include "../../kipod.h"
#include "softrenderer_uniform.h"
#include "softrenderer_layout.h"
#include "softrenderer_framebuffer.h"


namespace kipod{
class Light;
class MaterialStruct;
class SoftrendererBuffer;

enum LineAlgo {
    DDA, MIDPOINT_SLOW, BREZENHAM, BREZENHAM_WIKI
};

class SoftRenderer
{
    std::unique_ptr<SoftRendererFramebuffer> framebuffer_;
    std::unique_ptr<SoftRendererUniform> uniform_;

    int *m_zbuffer; // width*height

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

    std::function<void(int, int)>  drawPointFunction;
    std::function<void(int*, int*)> drawStraightLineFunction;
    std::function<void(int, int, float*)> drawPointWithColorFunction;



public:
    SoftRenderer(int width, int height);
    ~SoftRenderer() {};

    void SetUniforms(RenderCamera* camera, mat4 transform);

    void DrawTriangles(RenderObject* model,
                       bool wireframeMode = true, bool clippingMode=true);

    void DrawColoredTriangles(RenderObject* model,
                              const LightContainer &lights,
                              bool emissiveMode=false);

    void DrawToOpenGL();
    void ClearColorBuffer();
    void ClearDepthBuffer();
    void initialize_zBuffer(int zPrec=65535);
    void ClearBuffer();

    void drawPoint(int x, int y);
    void drawPointWithColor(int x, int y, float* c);
    void drawStraightLine(int *p, int *q);

    LineAlgo lineAlgo = BREZENHAM;
    bool clippingMode = true;

    void drawLineCall(int* p, int* q);
    void drawTriangleCall(int triangle[3][2]);


};

}
