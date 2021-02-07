#include "shapes.h"




void Shape::MakeFan()
{
    triangleFan_.push_back(center_);
    for(auto& v : transformed_vertices_)
        triangleFan_.push_back(v);
    triangleFan_.push_back(transformed_vertices_[0]);
}

Shape::Shape(Polygon polygon): Polygon(polygon)
{    
}


void Shape::Init()
{
    //MakeFan();
    std::string name = "Shape";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupShape(&transformed_vertices_);
    AddLayout(name, layout);

}

void Shape::Draw()
{
    RenderObject::Draw("Shape");
}


void Shape::ScaleShape(const float scale)
{
    world_transform_ = Scale(vec3(scale,scale,1)) * world_transform_;
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




