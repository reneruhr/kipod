#pragma once

#include "../core.h"

#include "engine_input.h"

namespace kipod{

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
