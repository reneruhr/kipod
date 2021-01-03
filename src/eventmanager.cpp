#include "../include/eventmanager.h"

EventManager::EventManager()
{

}

void EventManager::requestUpdate()
{

}

void EventManager::dispatch(Event event)
{
    events.push_back(event);
}

void EventManager::process()
{
    while(!std::empty(events)){
        auto& e = events.back();
        for(auto l : listeners)     l->processEvent(e);
        events.pop_back();
    }
}

void EventManager::addListener(Listener* id)
{
    listeners.push_back(id);
}
