#pragma once
#include "../engine/engine_module.h"
#include "../entity/entity.h"

namespace kipod
{
auto BaseShader();

const auto render_component = MakeHash("RenderObject");

class BasicActor : public Actor
{
public:
    explicit BasicActor(std::string name) : Actor(std::move(name)) {}

    template<typename Vectors>
    BasicActor(std::string name, const Vectors &vectors, GLenum primitive)
    :
    Actor(std::move(name))
    {
        auto object = AddComponent(BasicObject(name, vectors, primitive ));
        auto layout = GLRenderLayout{};
        layout.SetupLayout(vectors, primitive);
        object->AddLayout(name, std::move(layout));
        object->SetColor(glm::vec3(1.f));
    }

    template<typename Vectors>
    BasicActor(std::string name, const Vectors &vectors, const std::vector<unsigned int>& indices, GLenum primitive)
    :
    Actor(std::move(name))
    {
        auto object = AddComponent(BasicObject(name, vectors, indices, primitive ));
        auto layout = GLRenderLayout{};
        layout.SetupLayout(vectors, primitive);
        object->AddLayout(name, std::move(layout));
        object->SetColor(glm::vec3(1.f));
    }

};

class BasicObject : public RenderObject, public Component
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
};

using RenderObjectWPtr = std::weak_ptr<RenderObject>;
using RenderObjectSPtr = std::shared_ptr<RenderObject>;

class BasicScene : public RenderScene, public Listener, public Controls
{
    using Objects = std::vector<std::weak_ptr<RenderObject>>;
    using Shaders = std::vector<std::shared_ptr<Shader>>;
    Objects objects_{};
    Shaders shaders_{};
    bool mouse_rotation_active_ = false;
    RenderObjectWPtr active_object_;
public:
    BasicScene(int w, int h) : RenderScene(w,h) {}

//    template<typename Vectors, typename Type>
//    decltype(auto) Add(const Vectors &vectors, Type primitive)
//    {
//        objects_.template emplace_back(std::make_shared<BasicObject>(BasicObject("Object", vectors, primitive)));
//        return objects_.back();
//    }
//
//    template<typename Vectors, typename Type>
//    decltype(auto) Add(const std::vector<unsigned int>& indices, const Vectors &vectors, Type primitive)
//    {
//        objects_.template emplace_back(std::make_shared<BasicObject>(BasicObject("Object", vectors, indices, primitive)));
//        return objects_.back();
//    }


    void Add(const std::shared_ptr<RenderObject>& object);
    void Add(std::weak_ptr<Actor> actor);
    void Add(const std::shared_ptr<Shader>& shader) {shaders_.push_back(shader); }
    auto GetActiveObject() ->RenderObject* { return RenderObjectSPtr(active_object_).get(); }
    template <typename ObjectPtr>
    void SetActiveObject(ObjectPtr&& object) { active_object_ = object; }

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

using Actors = std::vector<ActorSPtr>;

class BasicModule : public Module
{
    Actors actors_;
public:
    BasicModule(int width, int height, std::string name = "Basic Module");

    template<typename ActorT, typename ...Args>
    auto Add(Args&&... args) -> std::weak_ptr<ActorT>
    {
        auto new_actor = std::make_shared<ActorT>(std::forward<Args...>(args...));
        actors_.push_back(new_actor);
        return actors_.back();
    }
};
}