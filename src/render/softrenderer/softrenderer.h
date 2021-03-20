#pragma once
#include "../../core.h"
#include "../../kipod.h"
#include "softrenderer_uniform.h"
#include "softrenderer_layout.h"


namespace kipod{
class Light;
class MaterialStruct;
class SoftrendererBuffer;

enum LineAlgo {
    DDA, MIDPOINT_SLOW, BREZENHAM, BREZENHAM_WIKI
};

class SoftRenderer
{
    float *m_outBuffer; // 3*width*height
    int *m_zbuffer; // width*height

    int width_, height_;

    void CreateBuffers();
    void CreateLocalBuffer();

    GLuint program;
    GLuint buffer;
    GLint  vPosition;
    GLint  vTexCoord;

    GLuint gScreenTex;
    GLuint gScreenVtc;
    void CreateOpenGLBuffer();
    void InitOpenGLRendering();

    std::function<void(int, int)>  drawPointFunction;
    std::function<void(int*, int*)> drawStraightLineFunction;
    std::function<void(int, int, float*)> drawPointWithColorFunction;

    SoftRendererUniform* uniform_;

public:
    SoftRenderer(int width, int height);
    ~SoftRenderer() { delete uniform_; };

    void Init();

    void SetUniforms(RenderCamera* camera, mat4 transform);

    void DrawTriangles(RenderObject* model,
                       bool wireframeMode = true, bool clippingMode=true);
    void DrawColoredTriangles(RenderObject* model,
                              const std::vector<RenderMaterial> *colors, const std::vector<unsigned int> *cindices,
                              const std::vector<RenderLight*> &lights,
                              bool lightMode=true, bool emissiveMode=false);

    void SwapBuffers();
    void ClearColorBuffer();
    void ClearDepthBuffer();
    void SetDemoBuffer();
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
