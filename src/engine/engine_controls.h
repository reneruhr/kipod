#pragma once

#include "../core.h"

#include "engine_input.h"

namespace kipod{



struct MultipleModeToggle
{
    MultipleModeToggle() = default;
    MultipleModeToggle(std::vector<std::string> toggle_names)
        : toggle_names_(toggle_names){}

    operator const int () const { return state_; }
    operator int& () { return state_; }


    entt::delegate<void(void*, int)> delegate_;
    int state_ = -1;
    std::string name_;
    bool verbose_ = true;

    std::vector<std::string> toggle_names_;

    void Set(int state);
    void Apply(void *ptr){ delegate_(ptr, state_); };
};

inline std::ostream& operator<<(std::ostream& os, const MultipleModeToggle& mode)
{
    return os << "Mode" << mode.name_ << " is set " << mode.state_;
}

struct ModeToggle
{
    ModeToggle() = default;
    ModeToggle(std::string name, bool state) : state_(state), name_(name){};
    ModeToggle(std::string name) : name_(name){};

    operator const bool () const { return state_; }

    bool state_ = false;
    std::string name_;
    bool verbose_ = true;

    void On();
    void Off();
    void Switch();
    void Set(bool state);
    void Set(int state);

    virtual void Apply(){ LOG_ENGINE("Mode {} is {}", name_, state_); }
};

inline std::ostream& operator<<(std::ostream& os, const ModeToggle& mode)
{
    return os << "Mode" << mode.name_ << " is set " << mode.state_;
}


class Controls
{
public:
    std::unordered_map<std::string, ModeToggle> mode_toggles_;

    void Add(ModeToggle&& toggle){ mode_toggles_.emplace(toggle.name_, toggle); }

    ModeToggle& Toggle(std::string toggle){ return mode_toggles_[toggle];   }

    virtual void ProcessKeys(KeyPressedEvent&) {};
};

}
