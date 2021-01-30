#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "eventmanager.h"

namespace kipod{
class Window;
}
class GLFWwindow;


class InputManager
{
public:
    InputManager(EventManager* eventmanager);
    void init(kipod::Window *window);
private:
    EventManager* eventmanager;
};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
#endif // INPUTMANAGER_H
