#pragma once

#include "../core.h"
#include "../glrenderer.h"
#include "../math/polygon.h"

#include "render_object.h"

class Shape : public Polygon, public kipod::RenderObject
{
    void MakeFan();

protected:
    mat4 world_transform_;

public:
    shared_ptr<ShapeData> shape_data_ = nullptr;
    vector<vec2> triangleFan_;
    float depth_ = -0.1f;

    Shape(){};
    Shape(Polygon polygon);

    void Init();

    void ScaleShape(const float scale);
    void Move(const vec2& translate);
    void MoveWorld(const mat4& transform);
    mat4 GetWorldTransform();

    void SetUniformMaterial(MaterialStruct &material);
    void SetUniformMaterial();


};
