#pragma once

#include "../core.h"


namespace kipod{


using namespace std::chrono_literals;
using Time = std::chrono::high_resolution_clock;


class Clock
{
    std::chrono::milliseconds target_time_step_ = 16ms;
    std::chrono::milliseconds current_time_step_;

    std::chrono::time_point<Time> last_;

public:
    Clock() : last_(Time::now()) {}

    int Timestep ();
    void Synchronize();
};

}

