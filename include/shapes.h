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


class Shape : public Polygon
{

    void MakeFan();

protected:
    mat4 world_transform_;
public:
    Shape(){};
    Shape(Polygon polygon);


    shared_ptr<ShapeData> shape_data_ = nullptr;

    vector<vec2> triangleFan_;

    void Init(GLRenderer *glrenderer);
    void Draw(GLRenderer *glrenderer);

    void Move(const vec2& translate);
    void MoveWorld(const mat4& transform);
    mat4 GetWorldTransform();

    void SetUniformMaterial(MaterialStruct &material);
    void SetUniformMaterial();

};

#endif // SHAPES_H
