#pragma once

#include "../core.h"
#include "engine_events.h"
#include "../utils/key_table.h"

namespace kipod{


class Input
{
    inline static double x_last =0;
    inline static double y_last =0;
public:
    static void KeyBoard(int key, int scancode, int action, int mods);
    static void MouseButton(int button, int action, int mods);
    static void MousePosition(double x, double y);
};


class MouseMoveEvent : public Event{
public:
    EVENT_CLASS_CATEGORY(EventCategoryMouse)
    EVENT_CLASS_TYPE(MouseMoved)
    MouseMoveEvent(const double x, const double y)
        : x_offset_(x), y_offset_(y){}

    double x() {return x_offset_;}
    double y() {return y_offset_;}

private:
    double x_offset_;
    double y_offset_;
};

class MouseButtonEvent : public Event{
public:
    EVENT_CLASS_CATEGORY(EventCategoryMouseButton)
    MouseButtonEvent(const MouseButton button, const Mod mod = Mod::None)
        : button_(button), mod_(mod){}

    MouseButton GetButton() {return button_;}
    Mod GetMod() {return mod_;}

protected:
    MouseButton button_;
    Mod mod_;
};

class MouseButtonPressEvent : public MouseButtonEvent{
public:
    MouseButtonPressEvent(const MouseButton button, const ::Mod mod = Mod::None)
        : MouseButtonEvent(button, mod){}
        EVENT_CLASS_TYPE(MouseButtonPressed)
        std::string ToString() const override;
};

class MouseButtonReleaseEvent : public MouseButtonEvent{
public:
    MouseButtonReleaseEvent(const MouseButton button, const ::Mod mod = Mod::None)
        : MouseButtonEvent(button, mod){}
        EVENT_CLASS_TYPE(MouseButtonReleased)
        std::string ToString() const override;
};



class KeyEvent : public Event
{
public:
    Key GetKeyCode() const { return m_KeyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
    KeyEvent(const Key keycode)
        : m_KeyCode(keycode) {}

    Key m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(const Key keycode, const uint16_t repeatCount, const Mod mod = Mod::None)
        : KeyEvent(keycode), m_RepeatCount(repeatCount), mod_(mod) {}

    uint16_t GetRepeatCount() const { return m_RepeatCount; }
    Mod GetMod() const { return mod_; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << static_cast<int>(m_KeyCode) << " (" << m_RepeatCount << " repeats), Mod: " << static_cast<int>(mod_) ;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
private:
    uint16_t m_RepeatCount;
    Mod mod_;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(const Key keycode)
        : KeyEvent(keycode) {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << static_cast<int>(m_KeyCode);
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
    KeyTypedEvent(const Key keycode)
        : KeyEvent(keycode) {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << static_cast<int>(m_KeyCode);
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};


}
