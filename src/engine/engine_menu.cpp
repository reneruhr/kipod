#include "engine_menu.h"
#include "engine_menu_events.h"
#include "engine_application.h"


namespace kipod{

void Menu::DrawFiles()
{
    if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close", NULL, false, true))
                    Events::Add<MenuEvent>({MenuEventTypes::AppClose});
                ImGui::EndMenu();
            }
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
                if(ImGui::MenuItem(name.c_str(),  "", name == active_module))  app_->ActiveModule(name);
            ImGui::EndMenu();
        }           
        ImGui::EndMenuBar();
    }
}

void Menu::DrawTools()
{
    static bool show_demo_ = false;
    if(show_demo_) ImGui::ShowDemoWindow();

    if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("ImGui Demo", NULL, show_demo_, true))
                    show_demo_ = !show_demo_;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
    }
}

}
