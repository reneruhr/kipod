#include "engine_sidebar.h"
#include "engine_gui.h"
#include "engine_events.h"

namespace kipod{

void Sidebar::Draw()
{
    int width = ImGui::GetWindowSize().y;
    if(width!=width_){

    }

    Gui::BeginWindow("Module Sidebar");

    if(ImGui::IsWindowFocused() ||
       ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_AllowWhenBlockedByPopup))
       kipod::Events::BlockMouse(true);
//    else kipod::Events::BlockMouse(false);

    SideBarContent();

    Gui::EndWindow();
}

}
