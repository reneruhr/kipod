#include "templates_scene.h"

void kipod::Templates::TemplateScene::Setup()
{
        name_ = "Template Scene";

        SetupShaders();
        Signup();

        LOG_INFO("Template Scene Initialized.");
        LOG_CONSOLE("Template Scene Initialized.");
}

void kipod::Templates::TemplateScene::Draw()
{
    framebuffer_->Bind();
    glViewport(0, 0, width_, height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    shaders_["Template"]->Use();
    for(auto& template_object : templates_){
        SetupUniforms(template_object.get());
        template_object->Draw();
    }

    glDisable(GL_DEPTH_TEST);

    kipod::RenderManager::Bind(0);

}

void kipod::Templates::TemplateScene::AddTemplateObject(TemplateObject&& template_object)
{
    templates_.push_back(
                std::make_unique<TemplateObject>(
                    std::forward<TemplateObject>(template_object)));
}

void kipod::Templates::TemplateScene::ActiveTemplateObject(
        TemplateObject* active)
{
    active_template_object_= active;
}

kipod::Templates::TemplateObject* kipod::Templates::TemplateScene::ActiveTemplateObject()
{
    return active_template_object_;
}


bool kipod::Templates::TemplateScene::HasTemplateObject()
{
    return active_template_object_!=nullptr;
}

void kipod::Templates::TemplateScene::ProcessKeys(kipod::KeyPressedEvent &event)
{
    float stepsize = 1.0f;
    auto key = event.GetKeyCode();

    if(HasTemplate()){
        if(key == Key::Left){
                    ActiveTemplate()->world_->Translate({-stepsize,0,0});
                }
        else if(key == Key::Right){
                    ActiveTemplate()->world_->Translate({+stepsize,0,0});
                }
        else if(key == Key::Up){
                    ActiveTemplate()->world_->Translate({0,stepsize,0});
                }
        else if(key == Key::Down){
                    ActiveTemplate()->world_->Translate({0,-stepsize,0});
                }
        else if(key == Key::PageUp){
                    ActiveTemplate()->world_->Translate({0,0, stepsize});
                }
        else if(key == Key::PageDown){
                    ActiveTemplate()->world_->Translate({0,0,-stepsize});
                }
    }
}

void kipod::Templates::TemplateScene::SetupLayout(TemplateObject* template_object)
{
    template_object->Init();
}

void kipod::Templates::TemplateScene::SetupShaders()
{
    shaders_.insert({"Template", std::make_shared<kipod::Shader>("template.vert.glsl",   "template.frag.glsl")});
    shaders_["Template"]->AttachUniform<glm::mat4>("transform");
}

void kipod::Templates::TemplateScene::SetupUniforms(TemplateObject *template_object)
{
    shaders_["Template"]->SetUniform<glm::mat4>("transform", (glm::mat4)*GetActiveCamera()*template_object->TransformWorld());
}


kipod::Templates::TemplateScene::TemplateScene(int width, int height):
    RenderScene(width, height), templates_(){}

void kipod::Templates::TemplateScene::Signup()
{
    kipod::Events::Signup(this, kipod::EventCategoryKeyboard); LOG_CATEGORY_NAME(kipod::EventCategoryKeyboard);
}

void kipod::Templates::TemplateScene::Receive(std::shared_ptr<kipod::Event> event)
{
    Process<kipod::KeyPressedEvent>(event, BIND_EVENT_FN(TemplateScene::ProcessKeys));
}
