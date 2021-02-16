#include "../core.h"
#include "engine_input.h"
#include "engine_events.h"


void kipod::Input::KeyBoard(int key, int scancode, int action, int mods)
{
   // EventManager *eventmanager;
    //eventmanager = reinterpret_cast<kipod::Window*>(glfwGetWindowUserPointer(window))->eventmanager;
    LOG_ENGINE("Got key {}", key);
    if (action == GLFW_PRESS && !Events::BlockKeyboard() )

        Events::Add<KeyPressedEvent>({Key(key),0});

 //   if (action == GLFW_PRESS){
 //       LOG_DEBUG("PUSHED BUTTON {}", key);
 //       eventmanager->dispatch(Event(EventType::SceneUpdate, Mode::ON));
//        switch ( key ) {

//            case GLFW_KEY_W: {
//                Event MoveCameraRight( EventType::TranslateCamera, MoveDirection::RIGHT);
//                eventmanager->dispatch(MoveCameraRight);
//                break; }
////            case GLFW_KEY_S:
////                translateEye=-e.x;
////                scene->moveEyeOfCamera(scene->activeCamera, translateEye);
////                break;
////            case GLFW_KEY_A:
////                translateEye=-e.y;
////                scene->moveEyeOfCamera(scene->activeCamera, translateEye);
////                break;
////            case GLFW_KEY_D:
////                translateEye=e.y;
////                scene->moveEyeOfCamera(scene->activeCamera, translateEye);
////                break;
////            case GLFW_KEY_R:
////                translateEye=e.z;
////                scene->moveEyeOfCamera(scene->activeCamera, translateEye);
////                break;
////            case GLFW_KEY_F:
////                translateEye=-e.z;
////                scene->moveEyeOfCamera(scene->activeCamera, translateEye);
////                break;
//            case GLFW_KEY_UP:
//                eventmanager->dispatch(Event(EventType::TranslateModel, MoveDirection::UP));
//                break;
//            case GLFW_KEY_DOWN:
//                eventmanager->dispatch(Event(EventType::TranslateModel, MoveDirection::DOWN));
//                break;
//            case GLFW_KEY_LEFT:
//                eventmanager->dispatch(Event(EventType::TranslateModel, MoveDirection::LEFT));
//                break;
//            case GLFW_KEY_RIGHT:
//                eventmanager->dispatch(Event(EventType::TranslateModel, MoveDirection::RIGHT));
//                break;
//            case  GLFW_KEY_PAGE_UP:{
//                Event MoveModelForward( EventType::TranslateModel, MoveDirection::FORWARD);
//                eventmanager->dispatch(MoveModelForward);
//                break;}
//            case  GLFW_KEY_PAGE_DOWN:{
//                Event MoveModelBackward( EventType::TranslateModel, MoveDirection::BACKWARD);
//                eventmanager->dispatch(MoveModelBackward);
//                break;}


////            case GLFW_KEY_F5:{
////                static bool debugmode = true;
////                debugmode=!debugmode;
////                if(debugmode) {
////                    std::cout << "DEBUG ON" << std::endl;
////                } else{
////                    std::cout << "DEBUG OFF" << std::endl;
////                }
////                break;
////                }

//        }
//    }

}
