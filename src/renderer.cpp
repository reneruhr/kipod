#include "renderer.h"
#include "core.h"
#include "scene.h"
#include "meshmodel.h"
#include "glrenderer.h"
#include "render/initshader.h"


void Renderer::SetCameraTransform(const mat4& cTransform){
    _cTransform=cTransform;
}
void Renderer::SetProjection(const mat4& projection){
    _projection=projection;
}
void Renderer::SetObjectMatrices(const mat4& oTransform, const mat3& nTransform){
    _oTransform=oTransform;
    _nTransform=nTransform;
}

void Renderer::SetMVP(){
    mvp = _projection * _cTransform * _mTransform;
}

void Renderer::SetVP(){
    mvp = _projection * _cTransform;
}

void Renderer::SetModelTransform(const mat4& mTransform){
    _mTransform = mTransform;
}

