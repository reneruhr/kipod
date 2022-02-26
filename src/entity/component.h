#pragma once
#include <memory>
namespace kipod {

class Actor;
class Component;

using ActorSPtr = std::shared_ptr<Actor>;
using ActorWPtr = std::weak_ptr<Actor>;
using ComponentSPtr = std::shared_ptr<Component>;
using ComponentName = std::size_t;


template<typename T, typename ... Args>
auto MakeComponent(Args&& ... args) -> ComponentSPtr
{
    return std::make_shared<T>(std::forward<Args...>(args ...));
}

class Component
{
    ActorSPtr actor_;
    ComponentName name_;
public:

    virtual ~Component() = default;

    auto GetActor() -> ActorWPtr { return actor_; };
    auto GetName() -> ComponentName { return name_; }
};

}