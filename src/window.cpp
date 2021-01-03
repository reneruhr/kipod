#include "../include/window.h"
#include "../include/core.h"

unsigned int GLOBAL_SCR_WIDTH = 800;
unsigned int GLOBAL_SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Window::Window(unsigned int width, unsigned int height, std::string title)
    : _width(width), _height(height), _title(title)
{

}

Window::~Window()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

int Window::init()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    _window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
    glfwSetWindowAspectRatio(_window, _width, _height);

    if (_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(_window);

    glfwSetKeyCallback(_window, key_callback);
    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetMouseButtonCallback(_window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

    glfwSetWindowUserPointer(_window, reinterpret_cast<void*>(this));

    glfwSwapInterval(1);


    glewExperimental = GL_TRUE;
    glewInit();
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        /*		...*/
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    return 0;
}

bool Window::windowShouldClose()
{
    return glfwWindowShouldClose(_window);
}

void Window::updateWindow(){
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void Window::SplitScreen(bool active)
{
    if(active == splitScreen)   return;


    if(splitScreen) _width /= 2;
    else _width *= 2;

    glfwSetWindowAspectRatio(_window, _width, _height);
    splitScreen = !splitScreen;
    glfwSetWindowSize(_window,_width,_height);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    //glViewport(0, 0, width, height);
    bool splitScreen = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->splitScreen;
    GLOBAL_SCR_WIDTH = width/2 * ( splitScreen? 1 : 2 );
    GLOBAL_SCR_HEIGHT = height;
}
