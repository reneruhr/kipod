#pragma once
#include "../../core.h"
#include "shapes_polygon.h"
#include "../../render/render_object.h"

class ShapeData;
class MaterialStruct;

namespace kipod::Shapes{

class Shape : public Polygon, public kipod::RenderObject
{
    void MakeFan();

protected:
    mat4 world_transform_;

public:
    std::shared_ptr<ShapeData> shape_data_ = nullptr;
    std::vector<vec2> triangleFan_;
    float depth_ = -0.1f;

    Shape(){};
    Shape(Polygon polygon);

    void Init();

    void ScaleShape(const float scale);
    void ScaleShape(const float x, const float y);
    void Move(const vec2& translate);
    void MoveWorld(const mat4& transform);
    mat4 GetWorldTransform();

    void SetUniformMaterial(MaterialStruct &material);
    void SetUniformMaterial();
};

}
