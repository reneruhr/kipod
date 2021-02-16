#include "engine_console.h"
#include "engine_gui.h"



namespace kipod{

ExampleAppConsole &Console::GetConsole()
{
    static ExampleAppConsole console;
    return console;
}

void Console::Draw()
{

    //Gui::BeginWindow("Module Console");

    GetConsole().Draw("Module Console", (bool*)true);
    //ConsoleContent();
    //Gui::EndWindow();
}

}
