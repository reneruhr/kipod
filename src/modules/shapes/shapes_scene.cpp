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
    active_shape_=shapes_.insert(std::make_unique<Shape>(std::forward<Shape>(shape)));
}

void kipod::Shapes::ShapesScene::ActiveShape(
        std::multiset<std::unique_ptr<Shape>>::iterator active)
{
    active_shape_= active;
}

kipod::Shapes::Shape *kipod::Shapes::ShapesScene::ActiveShape()
{
    return active_shape_->get();
}


bool kipod::Shapes::ShapesScene::HasShape()
{
    return active_shape_!=std::end(shapes_);
}

void kipod::Shapes::ShapesScene::ProcessKeys(kipod::KeyPressedEvent &event)
{
    float stepsize = 1.0f;
    auto key = event.GetKeyCode();

    if(HasShape()){
        if(key == Key::Left){
                    ActiveShape()->world_->Translate({-stepsize,0,0});
                }
        else if(key == Key::Right){
                    ActiveShape()->world_->Translate({+stepsize,0,0});
                }
        else if(key == Key::Up){
                    ActiveShape()->world_->Translate({0,stepsize,0});
                }
        else if(key == Key::Down){
                    ActiveShape()->world_->Translate({0,-stepsize,0});
                }
        else if(key == Key::PageUp){
                    ActiveShape()->world_->Translate({0,0, stepsize});
                }
        else if(key == Key::PageDown){
                    ActiveShape()->world_->Translate({0,0,-stepsize});
                }
    }
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
    shaders_["Shape"]->SetUniform<glm::mat4>("transform", (glm::mat4)*GetActiveCamera()*shape->TransformWorld());
    shaders_["Shape"]->SetUniform<float>("depth", shape->depth_);
}

void kipod::Shapes::ShapesScene::SetupGrid2d()
{

}

void kipod::Shapes::ShapesScene::SetupOptions()
{

}

kipod::Shapes::ShapesScene::ShapesScene(int width, int height):
    RenderScene(width, height),
    shapes_([](const std::unique_ptr<Shape>& x, const std::unique_ptr<Shape>& y)
{
    return x->depth_ < y->depth_;
}){}

void kipod::Shapes::ShapesScene::Signup()
{
    kipod::Events::Signup(this, kipod::EventCategoryKeyboard); LOG_CATEGORY_NAME(kipod::EventCategoryKeyboard);
}

void kipod::Shapes::ShapesScene::Receive(std::shared_ptr<kipod::Event> event)
{
    Process<kipod::KeyPressedEvent>(event, BIND_EVENT_FN(ShapesScene::ProcessKeys));
}
