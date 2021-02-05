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


#endif // INPUTMANAGER_H
