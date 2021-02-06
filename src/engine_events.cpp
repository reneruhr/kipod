#include "../include/engine_events.h"


void kipod::Events::Process()
{
    for(auto event : events_){
        LOG_ENGINE("Got Event: {}", *event);
        for(auto& listeners : listeners_)
            if(event->IsInCategory(listeners.first))
                for(Listener& listener : listeners.second)
                    listener.Receive(event);
    }
    events_.clear();
}
