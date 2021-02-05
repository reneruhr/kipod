#include "../include/engine_events.h"


void kipod::Events::Process()
{
    std::vector< std::shared_ptr<Event> > leftover_events_;

    for(auto event : events_){
        LOG_ENGINE("Got Event: {}", *event);
        for(auto& listeners : listeners_)
            if(event->IsInCategory(listeners.first)){
                for(Listener& listener : listeners.second)
                    if(listener.Receive(event))
                        leftover_events_.push_back(event);
            }
    }

   std::swap(leftover_events_,events_);
}
