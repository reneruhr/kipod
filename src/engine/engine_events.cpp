#include "engine_events.h"


kipod::Events &kipod::Events::Get(){  static Events Events;   return Events;  }

void kipod::Events::Add(std::shared_ptr<kipod::Event> e)
{
    Events::Get().events_.push_back(e);
}

void kipod::Events::Signup(kipod::Listener* listener, kipod::EventCategory category)
{
    Events::listeners_[category].push_back(listener);
}

void kipod::Events::Remove(kipod::Listener* listener)
{
    for(auto& listeners : listeners_)
            listeners.second.remove(listener);
}

void kipod::Events::Process()
{
    for(auto event : events_){
        if(event->GetEventType()!=kipod::EventType::MouseMoved) LOG_DEBUG("Got Event: {}", *event);
        for(auto& listeners : listeners_)
            if(event->IsInCategory(listeners.first))
                for(auto& listener : listeners.second)
                    listener->Receive(event);
    }
    events_.clear();
}

void kipod::Listener::Remove()
{
    Events::Remove(this);
}
