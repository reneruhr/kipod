#include "engine_console.h"
#include "engine_gui.h"



namespace kipod{

AppConsole &Console::GetConsole()
{
    static AppConsole console;
    return console;
}

void Console::DrawAppConsole()
{
    GetConsole().Draw("Module Console", (bool*)true);
}


}
