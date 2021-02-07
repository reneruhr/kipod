#include "render_scene.h"
void kipod::RenderScene::SetupCoordinateAxis()
{
    coordinate_axis_ = std::make_unique<kipod::RenderObject>();
    std::vector<vec3> vertices = {vec3(-1,0,0),vec3(1,0,0),
                                  vec3(0,-1,0),vec3(0,1,0),
                                  vec3(0,0,-1),vec3(0,0,1),
                                 };
    std::vector<vec3> colors =   {vec3(0.7,0,0),vec3(1,0,0),
                                  vec3(0,0.7,0),vec3(0,1,0),
                                  vec3(0,0,0.7),vec3(0,0,1),
                                  };

    std::string name = "Coordinate Axis";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupLines(&vertices, &colors);
    layout->sha_ = new kipod::Shader("lines.vert.glsl", "lines.frag.glsl");
    layout->sha_->AttachUniform<glm::mat4>("mvp");
    coordinate_axis_->AddLayout(name, layout);
}

void kipod::RenderScene::DrawCoordinateAxis(kipod::RenderCamera* camera){
    auto layout = static_cast<kipod::GLRenderLayout*>(coordinate_axis_->Layout("Coordinate Axis"));
    layout->sha_->Use();
    layout->sha_->SetUniform<glm::mat4>("mvp", *camera);
    layout->Draw();
    layout->sha_->Unuse();
}
