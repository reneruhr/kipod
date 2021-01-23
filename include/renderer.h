#pragma once

#include <vector>
#include "utils/vec.h"
#include "utils/mat.h"
#include "utils/log.h"
#include "GL/glew.h"

#include "render_engine.h"

using namespace std;

class Renderer : public kipod::RenderEngine
{
protected:
    int _width, _height;

    mat4 _cTransform;
    mat4 _projection;
    mat4 _oTransform;
    mat3 _nTransform;
    mat4 _mTransform;

    mat4 mvp;
public:
    Renderer(int width=800, int height=600): _width(width), _height(height) { LOG_DEBUG("Called Render Constructor");}
    //~Renderer(void)
    ~Renderer(void){}

    virtual void drawPoint(int x, int y){}
    virtual void drawPointWithColor(int x, int y, float* c){}
    virtual void drawStraightLine(int *p, int *q){}

    void SetCameraTransform(const mat4& cTransform);
    void SetProjection(const mat4& projection);
    void SetObjectMatrices(const mat4& oTransform, const mat3& nTransform);
    void SetModelTransform(const mat4& mTransform);
    void SetVP();
    void SetMVP();
};



