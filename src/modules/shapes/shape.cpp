#include "shape.h"
#include "../../render/opengl/opengl_layout.h"

namespace kipod::Shapes{
using Vec3 = glm::vec3;
Shape::Shape(Polygon polygon): Polygon(polygon)
{    
}

void Shape::Init()
{
    std::string name = "Shape";
    auto layout = new kipod::GLRenderLayout;
    auto fan = MakeFan();
    //layout->SetupShape(&transformed_vertices_);
    layout->SetupShape(&fan);
    AddLayout(name, std::move(*layout) );
    mat_ = std::make_shared<RenderMaterial>();
}

void Shape::ScaleShape(const float scale)
{
    world_->Scale(Vec3(scale,scale,1));
}

void Shape::ScaleShape(const float x, const float y)
{
    world_->Scale(Vec3(x,y,1));
}

void Shape::Move(const Vec2 &translate)
{
    world_->Translate({translate.x,translate.y,0});
}

void Shape::UpdateShape()
{
    auto fan = MakeFan();
    unsigned long buffersize = fan.size()*sizeof(Vec2);
    static_cast<GLRenderLayout*>(Layout())->vbo_->Add(0, buffersize, (void*)fan.data());
    LOG_INFO("Load new Shape vertices ({}-many) into graphics buffer", transformed_vertices_.size());
    for(const auto& v : fan)
        LOG_INFO("({},{})",v.x,v.y);
}

std::vector<Vec2> Shape::MakeFan() // Makes a fan with origin vector for TRIANGLE_FAN at center_=0
{
    std::vector<Vec2> triangleFan_;
    triangleFan_.push_back(center_);
    for(const auto& v : transformed_vertices_)
        triangleFan_.push_back(v);
    triangleFan_.push_back(transformed_vertices_[0]);

    return triangleFan_;
}
}
