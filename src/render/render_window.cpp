#include "render_window.h"
#include "render_manager.h"
#include "opengl/opengl_engine.h"

unsigned int GLOBAL_SCR_WIDTH = 1024;
unsigned int GLOBAL_SCR_HEIGHT = 800;


namespace kipod{

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double x, double y);



Window::Window(unsigned int width, unsigned int height, std::string title)
    : _width(width), _height(height), _title(title)
{
}

Window::~Window()
{
    glfwDestroyWindow(_window);
}

int Window::init()
{
    if(!glfwInit()){
        LOG_ENGINE("GLFW not initialized.");
        return -1;
    }
    #ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glsl_version = "#version 410";
        opengl_version_ = 410;
    #else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glsl_version = "#version 450";
        opengl_version_ = 450;
    #endif
    LOG_ENGINE("Using OpenGL Version {}", opengl_version_);

    OpenGLEngine::SetWindow(this);
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
    glfwSetMouseButtonCallback(_window, mouse_button_callback);
    glfwSetCursorPosCallback(_window, cursor_position_callback);
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


    glfwSetWindowSizeLimits(_window, 400, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

    Signup();

    kipod::RenderManager::addFrameBuffer();

    return 0;
}

bool Window::windowShouldClose()
{
    return glfwWindowShouldClose(_window);
}

void Window::updateWindow()
{
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

bool Window::CloseWindow(KeyPressedEvent &e)
{
    if(e.GetKeyCode() == Key::Escape)
        glfwSetWindowShouldClose(_window, true);
    return false;
}
bool Window::CloseWindow(MenuEvent &e)
{
    if(e.GetMenuSelection() == MenuEventTypes::AppClose)
        glfwSetWindowShouldClose(_window, true);
    return false;
}

void Window::Signup()
{
    kipod::Events::Signup(this, EventCategoryKeyboard);
    kipod::Events::Signup(this, EventCategoryApplication);
}

void Window::Receive(std::shared_ptr<Event> event)
{
    Process<KeyPressedEvent>(event, BIND_EVENT_FN(Window::CloseWindow));
    Process<MenuEvent>(event, BIND_EVENT_FN(Window::CloseWindow));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //glViewport(0, 0, width, height);
    bool splitScreen = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))->splitScreen;
    GLOBAL_SCR_WIDTH = width/2 * ( splitScreen? 1 : 2 );
    GLOBAL_SCR_HEIGHT = height;
    LOG_DEBUG("Resized Window. Width {} and height {}", width, height);
}

void key_callback(GLFWwindow*, int key, int scancode, int action, int mods)
{
    Input::KeyBoard(key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow*, int button, int action, int mods)
{
    Input::MouseButton(button, action, mods);
}

void cursor_position_callback(GLFWwindow*, double x, double y)
{
    Input::MousePosition(x,y);
}

}
