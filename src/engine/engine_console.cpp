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
    GetConsole().Draw("Kipod Console", (bool*)true);
}

void Console::DrawModuleConsole()
{
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    Gui::BeginWindow("Module Console");
    ConsoleContent();
    if(ImGui::IsWindowFocused() || ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByPopup)){
       kipod::Events::BlockMouse(true);
       kipod::Events::BlockKeyboard(true);
    }
    /*else{
         kipod::Events::BlockMouse(false);
         kipod::Events::BlockKeyboard(false);
    }*/
    Gui::EndWindow();
}


}
