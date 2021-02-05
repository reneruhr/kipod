#include "engine_events.h"
#include "utils/key_table.h"

namespace kipod{


class Input
{
public:
    static void KeyBoard(int key, int scancode, int action, int mods);
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
    KeyPressedEvent(const Key keycode, const uint16_t repeatCount)
        : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

    uint16_t GetRepeatCount() const { return m_RepeatCount; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << static_cast<int>(m_KeyCode) << " (" << m_RepeatCount << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
private:
    uint16_t m_RepeatCount;
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
