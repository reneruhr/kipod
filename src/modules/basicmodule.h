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
        SetColor(glm::vec3(1.f));
    }
    template<typename Vectors>
    BasicObject(const std::string& name, const Vectors &vectors, const std::vector<unsigned int>& indices, GLenum primitive) {
        auto layout = GLRenderLayout{};
        layout.SetupLayout(indices, vectors, primitive);
        RenderObject::AddLayout(name, std::move(layout));
        SetColor(glm::vec3(1.f));
    }
    BasicObject() = default;
    void SetColor(const glm::vec3& color);
    void SetColor(const glm::vec4 &color);
    void Update()
    {
        update_callback_(this);
    }

    void SetUpdateCallback(std::function<void(BasicObject*)> callback) { update_callback_ = callback;}
private:
    std::function<void(BasicObject*)> update_callback_ = [](BasicObject*){};
};

class BasicScene : public RenderScene, public Listener, public Controls
{
    using Objects = std::vector<std::shared_ptr<BasicObject>>;
    using Shaders = std::vector<std::shared_ptr<Shader>>;
    Objects objects_{};
    Shaders shaders_{};
    bool mouse_rotation_active_ = false;
    BasicObject* active_object_ = nullptr;
    int speed {1};
public:
    BasicScene(int w, int h) : RenderScene(w,h) {}

    template<typename Vectors, typename Type>
    decltype(auto) Add(const Vectors &vectors, Type primitive)
    {
        objects_.template emplace_back(std::make_shared<BasicObject>(BasicObject("Object", vectors, primitive)));
        return objects_.back();
    }

    template<typename Vectors, typename Type>
    decltype(auto) Add(const std::vector<unsigned int>& indices, const Vectors &vectors, Type primitive)
    {
        objects_.template emplace_back(std::make_shared<BasicObject>(BasicObject("Object", vectors, indices, primitive)));
        return objects_.back();
    }

    void Add(const std::shared_ptr<BasicObject> &object);
    void Add(const std::shared_ptr<Shader>& shader) {shaders_.push_back(shader); }
    BasicObject* GetActiveObject() { return active_object_; }
    void Setup() override;
    void Draw() override;
    void Update() override;
    void Signup() override;
    void ProcessKeys(kipod::KeyPressedEvent &event) override;
    void Receive(std::shared_ptr<kipod::Event> event) override;
    void ProcessMouseButtons(kipod::MouseButtonEvent &event);
    void ProcessMouseMoves(kipod::MouseMoveEvent &event);
    void SetupKeys();
    void PrepareScreen();
    auto GetObject(std::size_t index) { return objects_[index]; }
    void SetSpeed(int frames_skipped) { speed = frames_skipped; }
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