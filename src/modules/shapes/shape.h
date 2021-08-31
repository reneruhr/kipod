#pragma once
#include "../../core.h"
#include "shapes_polygon.h"
#include "../../render/render_object.h"

class ShapeData;

namespace kipod::Shapes{

class Shape : public Polygon, public kipod::RenderObject
{
public:
    std::shared_ptr<ShapeData> shape_data_ = nullptr;

    float depth_ = -0.1f;

    Shape(){};
    Shape(Polygon polygon);
    //~Shape() = default; // Bug

    void Init();

    void ScaleShape(const float scale);
    void ScaleShape(const float x, const float y);
    void Move(const vec2& translate);

    void UpdateShape();

    std::vector<vec2> MakeFan();
};
}
