#pragma once

#include "core.h"



namespace kipod{


enum class EventType
{
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
    EventCategoryNone = 0,
    EventCategoryApplication    =  1 << 0,
    EventCategoryInput          =  1 << 1,
    EventCategoryKeyboard       =  1 << 2,
    EventCategoryMouse          =  1 << 3,
    EventCategoryMouseButton    =  1 << 4
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType()                 { return EventType::type; }\
                               virtual EventType GetEventType() const override  { return GetStaticType(); }\
                               virtual const char* GetName() const override     { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }



class Event
{
public:
    virtual ~Event() = default;

    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    bool IsInCategory(EventCategory category)
    {
        return GetCategoryFlags() & category;
    }

};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    return os << e.ToString();
}


#define LISTENER_SIGNUP(category_in)  virtual void Signup() override { Events::Signup(*this, category_in); LOG_ENGINE("Sign up for {}", #category_in);}
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

class Listener
{
public:
    virtual ~Listener() = default;
    virtual void Signup() = 0;
    virtual bool Receive(std::shared_ptr<Event> event) = 0;

    template<typename T, typename F>
    bool Process(std::shared_ptr<Event> event, const F& func) {
            if (event->GetEventType() == T::GetStaticType())
            {
                return func(static_cast<T&>(*event));
            }
            return false;
    }
};



class Events
{
    Events(){}


public:
    Events(Events const&) = delete;
    void operator=(Events const&)  = delete;

    inline static std::vector< std::shared_ptr<Event> > events_ = {};
    inline static std::unordered_map< EventCategory, std::vector< std::reference_wrapper<Listener> > > listeners_ = {};

    static Events& Get(){  static Events Events;   return Events;  }

    static void Add(std::shared_ptr<Event> e) {  Events::Get().events_.push_back(e); }
    template <typename T>
    static void Add(T&& e) {  Events::Get().events_.push_back(std::make_shared<T>(e)); }

    static void Signup(Listener& l, EventCategory category){ Events::listeners_[category].push_back(std::ref(l)); }

    static void Process();

};

}
