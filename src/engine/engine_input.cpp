
#include "engine_input.h"
#include "engine_events.h"


void kipod::Input::KeyBoard(int key, int scancode, int action, int mods)
{
    LOG_ENGINE("Got key {}", key);
    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && !Events::BlockKeyboard() )
        Events::Add<KeyPressedEvent>({Key(key),0, Mod(mods)});
}

void kipod::Input::MouseButton(int button, int action, int mods)
{
    if (action == GLFW_PRESS )
        Events::Add<MouseButtonPressEvent>({::MouseButton(button), Mod(mods)});
    else if (action == GLFW_RELEASE )
        Events::Add<MouseButtonReleaseEvent>({::MouseButton(button), Mod(mods)});
}

void kipod::Input::MousePosition(double x, double y)
{

    float x_offset = x - x_last;
    float y_offset = y_last - y;

    x_last = x;
    y_last = y;

    Events::Add<MouseMoveEvent>({x_offset,y_offset});
}

std::string kipod::MouseButtonReleaseEvent::ToString() const
{
    std::stringstream ss;
    ss << "MouseButtonReleased: " << static_cast<int>(button_);
    return ss.str();
}

std::string kipod::MouseButtonPressEvent::ToString() const
{
    std::stringstream ss;
    ss << "MouseButtonPressed: " << static_cast<int>(button_);
    return ss.str();
}
