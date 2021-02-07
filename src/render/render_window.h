#pragma once

#include "../core.h"
#include "../engine/engine_input.h"
#include "../engine/engine_events.h"

extern unsigned int GLOBAL_SCR_WIDTH;
extern unsigned int GLOBAL_SCR_HEIGHT;


namespace kipod{

class Window : public Listener
{
    unsigned int _width, _height;
    std::string _title;


public:
    Window(unsigned int width, unsigned int height, std::string title);
    ~Window();
    int init();
    GLFWwindow* _window;
    const char* glsl_version = "#version 450";
    bool windowShouldClose();
    void updateWindow();

    void SplitScreen(bool active);
    bool splitScreen = false;

    unsigned Width(){ return _width; }
    unsigned Height(){ return _height; }

    bool CloseWindow(KeyPressedEvent& e) {
        if(e.GetKeyCode() == Key::Escape)
            glfwSetWindowShouldClose(_window, true);
        return false;
    }

    LISTENER_SIGNUP(EventCategoryKeyboard)
    virtual void Receive(std::shared_ptr<Event> event) override{

        Process<KeyPressedEvent>(event, BIND_EVENT_FN(Window::CloseWindow));

    }

};

}

