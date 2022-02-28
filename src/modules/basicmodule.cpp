#include "basicmodule.h"
#include "../render/opengl/opengl_layout.h"
namespace kipod
{

auto BaseShader()
{
    return std::make_shared<Shader>("kipod/shaders/basic.vert.glsl", "kipod/shaders/basic.frag.glsl");
}

void BasicScene::SetupKeys()
{
    Controls::Add("Left", Key::Left);
    Controls::Add("Right", Key::Right);
    Controls::Add("Up", Key::Up);
    Controls::Add("Down", Key::Down);
    Controls::Add("Forward", Key::PageUp);
    Controls::Add("Backward", Key::PageDown);
}

void BasicScene::PrepareScreen() {
    framebuffer_->Bind();
    glViewport(0, 0, width_, height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BasicScene::ProcessMouseMoves(MouseMoveEvent &event) {
    if(mouse_rotation_active_){
        GetActiveCamera()->Rotate(event.x(),event.y());
    }
}

void BasicScene::ProcessMouseButtons(MouseButtonEvent &event) {
    auto button = event.GetButton();

    if(button == MouseButton::Button0 && event.GetEventType() == kipod::EventType::MouseButtonPressed)
        mouse_rotation_active_ = true;
    else if(button == MouseButton::Button0 && event.GetEventType() == kipod::EventType::MouseButtonReleased)
        mouse_rotation_active_ = false;
}

void BasicScene::Receive(std::shared_ptr<kipod::Event> event) {
    Process<kipod::KeyPressedEvent>(event, BIND_EVENT_FN(ProcessKeys));
    Process<kipod::MouseButtonPressEvent>(event, BIND_EVENT_FN(ProcessMouseButtons));
    Process<kipod::MouseButtonReleaseEvent>(event, BIND_EVENT_FN(ProcessMouseButtons));
    Process<kipod::MouseMoveEvent>(event, BIND_EVENT_FN(ProcessMouseMoves));
}

void BasicScene::ProcessKeys(KeyPressedEvent &event) {
    float stepsize = 1.0f;
    float stepsize_small = 0.2f;
    auto key = event.GetKeyCode();
    auto mod = event.GetMod();

    //TOGGLES:
    //TRANSFORM CONTROL:
    if(key == Key("Left")){
        if(GetActiveObject())
            GetActiveObject()->world_->Translate({-stepsize,0,0});
    }
    else if(key == Key("Right")){
        if(GetActiveObject())
            GetActiveObject()->world_->Translate({+stepsize,0,0});
    }
    else if(key == Key("Up")){
        if(GetActiveObject())
            GetActiveObject()->world_->Translate({0,stepsize,0});
    }
    else if(key == Key("Down")){
        if(GetActiveObject())
            GetActiveObject()->world_->Translate({0,-stepsize,0});
    }
    else if(key == Key("Forward")){
        if(GetActiveObject())
            GetActiveObject()->world_->Translate({0,0, stepsize});
    }
    else if(key == Key("Backward")){
        if(GetActiveObject())
            GetActiveObject()->world_->Translate({0,0,-stepsize});
    }
    //CAMERA CONTROL:
    if(key == Key::A && mod == Mod::KIPOD_MOD_CONTROL){
        GetActiveCamera()->Move({-stepsize,0,0});
    }
    else if(key == Key::D && mod == Mod::KIPOD_MOD_CONTROL){
        GetActiveCamera()->Move({+stepsize,0,0});
    }
    else if(key == Key::W && mod == Mod::KIPOD_MOD_CONTROL){
        GetActiveCamera()->Move({0,stepsize,0});
    }
    else if(key == Key::S && mod == Mod::KIPOD_MOD_CONTROL){
        GetActiveCamera()->Move({0,-stepsize,0});
    }
    else if(key == Key::R && mod == Mod::KIPOD_MOD_CONTROL){
        GetActiveCamera()->Move({0,0, stepsize});
    }
    else if(key == Key::F && mod == Mod::KIPOD_MOD_CONTROL){
        GetActiveCamera()->Move({0,0,-stepsize});
    }

    else if(key == Key::A && mod == Mod::None){
        GetActiveCamera()->Move(kipod::RenderCamera::Movement::LEFT, stepsize);
    }
    else if(key == Key::D && mod == Mod::None){
        GetActiveCamera()->Move(kipod::RenderCamera::Movement::RIGHT, stepsize);
    }
    else if(key == Key::W && mod == Mod::None){
        GetActiveCamera()->Move(kipod::RenderCamera::Movement::FORWARD, stepsize_small);
    }
    else if(key == Key::S && mod == Mod::None){
        GetActiveCamera()->Move(kipod::RenderCamera::Movement::BACKWARD, stepsize_small);
    }
    else if(key == Key::R && mod == Mod::None){
        GetActiveCamera()->Move(kipod::RenderCamera::Movement::UP, stepsize);
    }
    else if(key == Key::F && mod == Mod::None){
        GetActiveCamera()->Move(kipod::RenderCamera::Movement::DOWN, stepsize);
    }
}

void BasicScene::Signup() {
    kipod::Events::Signup(this, kipod::EventCategoryKeyboard); LOG_CATEGORY_NAME(kipod::EventCategoryKeyboard);
    kipod::Events::Signup(this, kipod::EventCategoryMouseButton); LOG_CATEGORY_NAME(kipod::EventCategoryMouseButton);
    kipod::Events::Signup(this, kipod::EventCategoryMouse); LOG_CATEGORY_NAME(kipod::EventCategoryMouse);
}

void BasicScene::Draw() {
    PrepareScreen();

    for(auto& m : objects_)
    {
        auto transform = m->Transform();
        auto mvp = cameras_[0]->projection_view_matrix_ * transform;
        shaders_[0]->SetUniform<glm::vec4>("color", m->mat_->ambient_);
        shaders_[0]->SetUniform<glm::mat4>("mvp", mvp);
        shaders_[0]->Use();
        m->Draw();
    }
    kipod::RenderManager::Bind(0);
}

void BasicScene::Setup() {
    SetupKeys();
    Add(BaseShader());
    AddCamera(RenderCamera({0.0f,0.0f,-10.0f}));
    SetActiveCamera(0);
    shaders_[0]->AttachUniform<glm::mat4>("mvp");
    shaders_[0]->AttachUniform<glm::vec4>("color");

    glEnable(GL_DEPTH_TEST);
    LOG_ENGINE("Finished BasicScene Setup");
}


void BasicScene::Add(const std::shared_ptr<BasicObject> &object)
{
    objects_.push_back(object);
    active_object_ = objects_.back().get();
}

void BasicScene::Update()
{
    int static step {1};
    if( (step %speed )==0) {
        for (auto &object: objects_) {
            object->Update();
        }
    }
    step++;
}


BasicModule::BasicModule(int width, int height, std::string name) : Module(name)
{
    scene_ = std::make_shared<BasicScene>(BasicScene(width, height));
    sidebar_ = std::make_unique<BasicSidebar>(BasicSidebar{scene_});
    console_ = nullptr;
    menu_ = std::make_unique<LorenzModuleMenu>();
}

void BasicObject::SetColor(const glm::vec3 &color)
{
    if(!mat_)
        mat_ = std::make_shared<RenderMaterial>(0.);
    mat_->ambient_ = glm::vec4(color,1.f);
}

void BasicObject::SetColor(const glm::vec4 &color)
{
    if (!mat_)
        mat_ = std::make_shared<RenderMaterial>(0.);
    mat_->ambient_ = glm::vec4(color);
}

}