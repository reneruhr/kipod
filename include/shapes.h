#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
#include "utils/vec.h"
#include "utils/mat.h"
#include "softrenderer.h"
#include "glrenderer.h"
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "eventmanager.h"

#include "math/polygon.h"

#include "render_object.h"

class Shape : public Polygon, public kipod::RenderObject
{

    void MakeFan();

protected:
    mat4 world_transform_;

public:
    Shape(){};
    Shape(Polygon polygon);


    shared_ptr<ShapeData> shape_data_ = nullptr;

    vector<vec2> triangleFan_;

    void Init();
    void Draw();

    void ScaleShape(const float scale);
    void Move(const vec2& translate);
    void MoveWorld(const mat4& transform);
    mat4 GetWorldTransform();

    void SetUniformMaterial(MaterialStruct &material);
    void SetUniformMaterial();

    float depth_ = -0.1f;
};

#endif // SHAPES_H
