#include "engine_clock.h"

int kipod::Clock::Timestep()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(current_time_step_).count();
}

void kipod::Clock::Synchronize()
{
    std::chrono::milliseconds work_time = std::chrono::duration_cast<std::chrono::milliseconds>(Time::now() - last_);

    if(work_time<= target_time_step_){
        std::this_thread::sleep_for(target_time_step_-work_time);
        current_time_step_ = target_time_step_;
    }
    else{
        current_time_step_ = work_time;
    }
    last_=Time::now();
}
