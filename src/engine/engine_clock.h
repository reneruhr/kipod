#pragma once

#include "../core.h"

namespace kipod{

class Clock
{
    float time_;
    float step_;
public:
    Clock(float now = 0.0f) : time_(now) {}

    float Now()
    {
        time_ = glfwGetTime();
        return time_;
    }

    float Lap()
    {
        step_ = glfwGetTime()-time_;
        return step_;
    }

    void SlowDownTime(float limit = 1/60.0)
    {
        if(Lap() < limit){
            std::this_thread::sleep_for( std::chrono::milliseconds( (int)(1000*(limit-step_)) ) );
        }
        time_ = glfwGetTime();
    }
};

}

