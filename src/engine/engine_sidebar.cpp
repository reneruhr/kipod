#include "engine_sidebar.h"
#include "engine_gui.h"

namespace kipod{

void Sidebar::Draw()
{
    int width = ImGui::GetWindowSize().y;
    if(width!=width_){

    }

    Gui::BeginWindow("Module Sidebar");
    SideBarContent();
    Gui::EndWindow();
}

}
