#pragma once
#include "../../../core.h"
#include "../../../kipod.h"


namespace kipod::MeshModels{
class Light;
class MaterialStruct;

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

protected:
    mat4 _cTransform;
    mat4 _projection;
    mat4 _oTransform;
    mat3 _nTransform;
    mat4 _mTransform;
    mat4 mvp;

public:
    SoftRenderer(int width, int height);
    virtual ~SoftRenderer() = default;

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

}
