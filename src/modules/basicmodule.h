#pragma once
#include "../engine/engine_module.h"

namespace kipod
{

auto BaseShader();

class BasicObject : public RenderObject
{
public:
    template<typename Vectors>
    BasicObject(const std::string& name, const Vectors &vectors, GLenum primitive) {
        auto layout = GLRenderLayout{};
        layout.SetupLayout(vectors, primitive);
        RenderObject::AddLayout(name, std::move(layout));
    }
    BasicObject() = default;
};

class BasicScene : public RenderScene, public Listener, public Controls
{
    using Objects = std::vector<std::shared_ptr<RenderObject>>;
    using Shaders = std::vector<std::shared_ptr<Shader>>;
    Objects objects_{};
    Shaders shaders_{};
    bool mouse_rotation_active_ = false;
    RenderObject* active_object_ = nullptr;
public:
    BasicScene(int w, int h) : RenderScene(w,h) {}

    template<typename Vectors, typename Type>
    void Add(const Vectors &vectors, Type primitive)
    {
        objects_.template emplace_back(std::make_shared<BasicObject>(BasicObject("Object", vectors, primitive)));
    }

    void Add(const std::shared_ptr<RenderObject>& object);
    void Add(const std::shared_ptr<Shader>& shader) {shaders_.push_back(shader); }
    RenderObject* GetActiveObject() { return active_object_; }
    void Setup() override;
    void Draw() override;
    void Signup() override;
    void ProcessKeys(kipod::KeyPressedEvent &event) override;
    void Receive(std::shared_ptr<kipod::Event> event) override;
    void ProcessMouseButtons(kipod::MouseButtonEvent &event);
    void ProcessMouseMoves(kipod::MouseMoveEvent &event);
    void SetupKeys();
    void PrepareScreen();

};


class BasicSidebar : public Sidebar
{
public:
    explicit BasicSidebar(std::shared_ptr<RenderScene> scene) : kipod::Sidebar(std::move(scene)) {}
    void SideBarContent() override {}
};

class LorenzModuleMenu : public MenuModule
{
protected:
    void Draw() override {}
};

class BasicModule : public Module
{
public:
    BasicModule(int width, int height, std::string name = "Basic Module");
};

}