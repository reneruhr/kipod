#include "shapes_scene.h"

void kipod::Shapes::ShapesScene::Setup()
{
        LOG_ENGINE("Seting up Shapes Scene.");
        name_ = "Shapes 2d";
        AddCamera(kipod::RenderCamera({0.0f,0.0f,10.0f},
                                      {0.0f,0.0f,0.0f},
                                      {0.0f,1.0f,0.0f},
                                      RenderCamera::ORTHOGONAL
                                      ));
        SetActiveCamera(0);
        SetupShaders();
        SetupGrid2d();
        SetupOptions();
        Signup();

        AddShape(Shape( kipod::Shapes::Octagon(sqrt(2)) ));
}

void kipod::Shapes::ShapesScene::Draw()
{
    framebuffer_->Bind();
    glViewport(0, 0, width_, height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    shaders_["Shape"]->Use();
    for(auto& shape : shapes_){
        SetupUniforms(shape.get());
        shape->Draw();
    }
    glDisable(GL_DEPTH_TEST);
    kipod::RenderManager::Bind(0);
}

void kipod::Shapes::ShapesScene::AddShape(Shape&& shape)
{
    shape.Init();
    shapes_.insert(std::make_unique<Shape>(std::forward<Shape>(shape)));
}

void kipod::Shapes::ShapesScene::SetupLayout(Shape* shape)
{
    shape->Init();
}

void kipod::Shapes::ShapesScene::SetupShaders()
{
    shaders_.insert({"Shape", std::make_shared<kipod::Shader>("shape.vert.glsl",   "shape.frag.glsl")});
    shaders_["Shape"]->AttachUniform<float>("depth");
    shaders_["Shape"]->AttachUniform<glm::mat4>("transform");
}

void kipod::Shapes::ShapesScene::SetupUniforms(Shape *shape)
{
    shaders_["Shape"]->SetUniform<glm::mat4>("transform", shape->TransformWorld());
    shaders_["Shape"]->SetUniform<float>("depth", shape->depth_);
}

void kipod::Shapes::ShapesScene::SetupGrid2d()
{

}

void kipod::Shapes::ShapesScene::SetupOptions()
{

}
