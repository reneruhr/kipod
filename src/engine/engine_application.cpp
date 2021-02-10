#include "engine_application.h"

kipod::Application::Application()
{

}

void kipod::Application::Init(int, char**)
{
    Log::Init();
    LOG_ENGINE("Logger Started.");
}

void kipod::Application::Run()
{

}
