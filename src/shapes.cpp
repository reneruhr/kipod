#include "../include/shapes.h"




void Shape::MakeFan()
{
    triangleFan_.push_back(center_);
    for(auto& v : vertices_)
        triangleFan_.push_back(v);
    triangleFan_.push_back(vertices_[0]);
}

Shape::Shape(Polygon polygon): Polygon(polygon)
{    
}

void Shape::Init(GLRenderer *glrenderer)
{
    MakeFan();
    shape_data_ =
             glrenderer->LoadShape(&triangleFan_);
}

void Shape::ScaleShape(const float scale)
{
    world_transform_ = Scale(vec3(scale,scale,0)) * world_transform_;
}

void Shape::Move(const vec2 &translate)
{
    world_transform_=Translate(translate)*world_transform_;
}

void Shape::MoveWorld(const mat4 &transform)
{
     world_transform_ = transform*world_transform_;
}

mat4 Shape::GetWorldTransform()
{
    return world_transform_;
}



void Shape::Draw(GLRenderer *glrenderer)
{
    glrenderer->DrawShape(shape_data_);
}


