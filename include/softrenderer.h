#ifndef SOFTRENDERER_H
#define SOFTRENDERER_H

#include "renderer.h"
#include <vector>
#include "utils/vec.h"
#include "utils/mat.h"
#include "GL/glew.h"
#include "light.h"
#include <functional>

enum LineAlgo {
    DDA, MIDPOINT_SLOW, BREZENHAM, BREZENHAM_WIKI
};

class SoftRenderer : public Renderer
{
    float *m_outBuffer; // 3*width*height
    int *m_zbuffer; // width*height

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



public:
    SoftRenderer();
    SoftRenderer(int width, int height);
//    ~SoftRenderer(void);

    void Init();
    void DrawTriangles(const std::vector<vec3>* vertices, const std::vector<unsigned int>* indices,
                       bool wireframeMode = true, bool clippingMode=true,
                       const std::vector<vec3>* normals=nullptr, const std::vector<unsigned int>* nindices=nullptr);

    void DrawColoredTriangles(const std::vector<vec3>* vertices, const std::vector<unsigned int>* indices,
                       const std::vector<vec3>* normals, const std::vector<unsigned int>* nindices,
                       const std::vector<MaterialStruct> *colors, const std::vector<unsigned int> *cindices,
                       const std::vector<Light*> &lights,
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

#endif // SOFTRENDERER_H
