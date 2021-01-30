#ifndef WINDOW_HELPER_H
#define WINDOW_HELPER_H

#include <core.h>
#include "inputmanager.h"
#include "eventmanager.h"

extern unsigned int GLOBAL_SCR_WIDTH;
extern unsigned int GLOBAL_SCR_HEIGHT;

namespace kipod
{

class Window
{
    unsigned int _width, _height;
    std::string _title;

//    void resize(unsigned int width, unsigned int height){
//        _width=width;
//        _height=height;
//    }

public:
    Window(unsigned int width, unsigned int height, std::string title);
    ~Window();
    int init();
    GLFWwindow* _window;
    const char* glsl_version = "#version 450";
    bool windowShouldClose();
    void updateWindow();

    EventManager* eventmanager;

    void SplitScreen(bool active);
    bool splitScreen = false;

    unsigned Width(){ return _width; }
    unsigned Height(){ return _height; }
};

}
#endif // WINDOW_H
