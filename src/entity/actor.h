#pragma once
#include <utility>
#include <unordered_map>
#include <string>
#include "../utils/hash.h"

namespace kipod
{

class Component;

using EntityId = std::size_t;
using ComponentName = std::size_t;
using ComponentSPtr = std::shared_ptr<Component>;
using ComponentWPtr = std::weak_ptr<Component>;
using Components = std::unordered_map<ComponentName, ComponentSPtr>;

class Actor
{
    EntityId id_;
    Components components_;

    void DeleteComponents() { components_.clear(); }
public:
    virtual ~Actor() { DeleteComponents(); };

    explicit Actor(std::string name) : id_(MakeHash(std::move(name))) {}

    template<typename ConcreteComponent>
    auto AddComponent(ConcreteComponent&& comp) -> ComponentWPtr
    {
        auto comp_ptr = std::make_shared<ConcreteComponent>(std::move(comp));
        components_.emplace(comp_ptr->Getname(), comp_ptr);
        return GetComponent(comp_ptr->GetName());
    }

    auto GetComponent(ComponentName name) -> ComponentWPtr
    {
        using std::end;
        auto comp = components_.find(name);
        if(comp == end(components_))
        {
            return ComponentWPtr();
        }
        else
        {
            return comp->second;
        }
    }

};

}