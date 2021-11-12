#pragma once
#include <utility>

#include "../core.h"
#include "engine_input.h"

namespace kipod{

struct MultipleModeToggle
{
    MultipleModeToggle() = default;
    explicit MultipleModeToggle(std::vector<std::string> toggle_names)
        : toggle_names_(std::move(toggle_names)){}

    explicit operator int () const { return state_; }
    explicit operator int& () { return state_; }

    entt::delegate<void(void*, int)> delegate_;
    int state_ = -1;
    std::string name_;
    bool verbose_ = true;

    std::vector<std::string> toggle_names_;

    void Set(int state);
    void Apply(void *ptr) const{ delegate_(ptr, state_); };
};

inline std::ostream& operator<<(std::ostream& os, const MultipleModeToggle& mode)
{
    return os << "Mode" << mode.name_ << " is set " << mode.state_;
}

struct ModeToggle
{
    ModeToggle() = default;
    ModeToggle(std::string name, bool state) : state_(state), name_(std::move(name)){}
    explicit ModeToggle(std::string name) : name_(std::move(name)){};

     operator bool () const { return state_; }

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
protected:
    std::unordered_map<std::string, kipod::Key> key_table_;
    std::unordered_map<std::string, ModeToggle> mode_toggles_;
public:
    void Add(ModeToggle&& toggle){ mode_toggles_.emplace(toggle.name_, toggle); }
    ModeToggle& Toggle(const std::string& toggle){
        if(mode_toggles_.find(toggle)==end(mode_toggles_)) LOG_ENGINE("Did not find Toggle {}", toggle);
        return mode_toggles_[toggle];   }


    void Add(const std::string& name, kipod::Key key) { key_table_[name] = key; };
    kipod::Key Key(const std::string& name) { return key_table_[name]; }
    virtual void ProcessKeys(KeyPressedEvent&) {};
};

}
