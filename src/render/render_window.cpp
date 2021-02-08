#include "render_window.h"
#include "render_manager.h"

unsigned int GLOBAL_SCR_WIDTH = 800;
unsigned int GLOBAL_SCR_HEIGHT = 600;


namespace kipod{

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);



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
    if(!glfwInit()){
        LOG_ENGINE("GLFW not initialized.");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    _window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);

    if (_window == NULL)
    {
        LOG_ENGINE("Failed to create GLFW window.");
        glfwTerminate();
        return -1;
    }
    glfwSetWindowAspectRatio(_window, _width, _height);

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
        LOG_ENGINE("Glew Error: {}", glewGetErrorString(err));
    }
    LOG_ENGINE("Status: Using GLEW {}", glewGetString(GLEW_VERSION));


    Signup();

    kipod::RenderManager::addFrameBuffer();

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
    //glViewport(0, 0, width, height);
    bool splitScreen = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->splitScreen;
    GLOBAL_SCR_WIDTH = width/2 * ( splitScreen? 1 : 2 );
    GLOBAL_SCR_HEIGHT = height;
}

void key_callback(GLFWwindow*, int key, int scancode, int action, int mods)
{
    Input::KeyBoard(key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow*, int button, int action, int mods)
{

}


}
