#include "engine_menu.h"
#include "engine_menu_events.h"


namespace kipod{


void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void Menu::DrawFiles()
{
    if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {

                ImGui::Separator();
                if (ImGui::MenuItem("Close", NULL, false, true))
                    Events::Add<MenuEvent>({MenuEventTypes::AppClose});
                ImGui::EndMenu();
            }
            HelpMarker(
                "When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n\n"
                " > if io.ConfigDockingWithShift==false (default):" "\n"
                "   drag windows from title bar to dock" "\n"
                " > if io.ConfigDockingWithShift==true:" "\n"
                "   drag windows from anywhere and hold Shift to dock" "\n\n"
                "This demo app has nothing to do with it!" "\n\n"
                "This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window. This is useful so you can decorate your main application window (e.g. with a menu bar)." "\n\n"
                "ImGui::DockSpace() comes with one hard constraint: it needs to be submitted _before_ any window which may be docked into it. Therefore, if you use a dock spot as the central point of your application, you'll probably want it to be part of the very first window you are submitting to imgui every frame." "\n\n"
                "(NB: because of this constraint, the implicit \"Debug\" window can not be docked into an explicit DockSpace() node, because that window is submitted as part of the NewFrame() call. An easy workaround is that you can create your own implicit \"Debug##2\" window after calling DockSpace() and leave it in the window stack for anyone to use.)"
            );
            ImGui::EndMenuBar();
    }

}

void Menu::DrawModuleMenu(std::unordered_map<std::string, std::unique_ptr<Module> > &modules, std::string& active_module)
{
    if (ImGui::BeginMenuBar())
               {
           if (ImGui::BeginMenu("Modules"))
           {
               for(auto& [name, module] : modules)
                   if(ImGui::MenuItem(name.c_str(),  "", name == active_module))  active_module=name;
               ImGui::EndMenu();
           }
           ImGui::EndMenuBar();
    }
}

}
