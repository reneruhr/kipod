#include "shape.h"
#include "../../render/opengl/opengl_layout.h"
namespace kipod::Shapes{



Shape::Shape(Polygon polygon): Polygon(polygon)
{    
}

void Shape::Init()
{
    std::string name = "Shape";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupShape(&transformed_vertices_);
    AddLayout(name, std::move(*layout) );
    mat_ = std::make_shared<RenderMaterial>();
}

void Shape::ScaleShape(const float scale)
{
    world_->Scale(vec3(scale,scale,1));
}

void Shape::ScaleShape(const float x, const float y)
{
    world_->Scale(vec3(x,y,1));
}

void Shape::Move(const vec2 &translate)
{
    world_->Translate({translate.x,translate.y,0});
}



std::vector<vec2> Shape::MakeFan() // Makes a fan with origin vector for TRIANGLE_FAN at center_=0
{
    std::vector<vec2> triangleFan_;
    triangleFan_.push_back(center_);
    for(auto& v : transformed_vertices_)
        triangleFan_.push_back(v);
    triangleFan_.push_back(transformed_vertices_[0]);

    return triangleFan_;
}

}