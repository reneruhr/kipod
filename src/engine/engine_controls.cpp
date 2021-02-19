#include "engine_controls.h"

namespace kipod{

void ModeToggle::On(){
    state_ = true;
    if(verbose_) Apply();
}

void ModeToggle::Off(){
    state_ = false;
    if(verbose_) Apply();
}

void ModeToggle::Switch(){
    state_ = !state_;
    if(verbose_) Apply();
}

void ModeToggle::Set(bool state){
    state_ = state;
    if(verbose_) Apply();
}

void ModeToggle::Set(int state){
    assert(state == 0 || state == 1 || state == -1);
    if(state == 0 || state == 1 ) state_ = state;
    if(state == -1) state_=!state_;
}

void MultipleModeToggle::Set(int state)
{
    state_ = state;
}


}
