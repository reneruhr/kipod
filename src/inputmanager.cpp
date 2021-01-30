#include "../include/inputmanager.h"
#include "../include/eventmanager.h"
#include "../include/core.h"
#include "render_window.h"
InputManager::InputManager(EventManager *eventmanager) : eventmanager(eventmanager)
{

}

void InputManager::init(kipod::Window* window){
    window->eventmanager = eventmanager;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    EventManager *eventmanager;
    eventmanager = reinterpret_cast<kipod::Window*>(glfwGetWindowUserPointer(window))->eventmanager;

    if (action == GLFW_PRESS){
        LOG_DEBUG("PUSHED BUTTON {}", key);
        eventmanager->dispatch(Event(EventType::SceneUpdate, Mode::ON));
        switch ( key ) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);
                break;
            case GLFW_KEY_W: {
                Event MoveCameraRight( EventType::TranslateCamera, MoveDirection::RIGHT);
                eventmanager->dispatch(MoveCameraRight);
                break; }
//            case GLFW_KEY_S:
//                translateEye=-e.x;
//                scene->moveEyeOfCamera(scene->activeCamera, translateEye);
//                break;
//            case GLFW_KEY_A:
//                translateEye=-e.y;
//                scene->moveEyeOfCamera(scene->activeCamera, translateEye);
//                break;
//            case GLFW_KEY_D:
//                translateEye=e.y;
//                scene->moveEyeOfCamera(scene->activeCamera, translateEye);
//                break;
//            case GLFW_KEY_R:
//                translateEye=e.z;
//                scene->moveEyeOfCamera(scene->activeCamera, translateEye);
//                break;
//            case GLFW_KEY_F:
//                translateEye=-e.z;
//                scene->moveEyeOfCamera(scene->activeCamera, translateEye);
//                break;
            case GLFW_KEY_UP:
                eventmanager->dispatch(Event(EventType::TranslateModel, MoveDirection::UP));
                break;
            case GLFW_KEY_DOWN:
                eventmanager->dispatch(Event(EventType::TranslateModel, MoveDirection::DOWN));
                break;
            case GLFW_KEY_LEFT:
                eventmanager->dispatch(Event(EventType::TranslateModel, MoveDirection::LEFT));
                break;
            case GLFW_KEY_RIGHT:
                eventmanager->dispatch(Event(EventType::TranslateModel, MoveDirection::RIGHT));
                break;
            case  GLFW_KEY_PAGE_UP:{
                Event MoveModelForward( EventType::TranslateModel, MoveDirection::FORWARD);
                eventmanager->dispatch(MoveModelForward);
                break;}
            case  GLFW_KEY_PAGE_DOWN:{
                Event MoveModelBackward( EventType::TranslateModel, MoveDirection::BACKWARD);
                eventmanager->dispatch(MoveModelBackward);
                break;}
            case GLFW_KEY_SPACE:{
                eventmanager->dispatch(Event(EventType::WireframeMode, Mode::SWITCH));
                break;
                }
            case GLFW_KEY_N:
                eventmanager->dispatch(Event(EventType::NormalMode, Mode::SWITCH));
                break;
            case GLFW_KEY_B:
                eventmanager->dispatch(Event(EventType::BoxMode, Mode::SWITCH));
            break;
            case GLFW_KEY_C:
                eventmanager->dispatch(Event(EventType::CameraMode, Mode::SWITCH));
            break;
            case GLFW_KEY_X:
                eventmanager->dispatch(Event(EventType::ClippingMode, Mode::SWITCH));
            break;

//            case GLFW_KEY_F5:{
//                static bool debugmode = true;
//                debugmode=!debugmode;
//                if(debugmode) {
//                    std::cout << "DEBUG ON" << std::endl;
//                } else{
//                    std::cout << "DEBUG OFF" << std::endl;
//                }
//                break;
//                }

        }
    }
}



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
//    if (button == GLFW_MOUSE_BUTTON_LEFT&& action == GLFW_PRESS)
//    {
//        if(line_active)
//        {
//            if(!first_point)
//            {
//                LOG_INFO("Get First Point");
//                glfwGetCursorPos(window, &x, &y);
//                screenToPixel_x(x,p[0]);
//                screenToPixel_y(y,p[1]);
//                LOG_INFO(p[0]);LOG_INFO(p[1]);
//                first_point = true;
//            }
//            else
//            {
//                LOG_INFO("Get Second Point");
//                glfwGetCursorPos(window, &x, &y);
//                screenToPixel_x(x,q[0]);
//                screenToPixel_y(y,q[1]);
//                LOG_INFO(q[0]);LOG_INFO(q[1]);
//                second_point=true;
//            }
//        }
//    }
}
