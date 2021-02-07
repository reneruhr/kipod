#include "guielement.h"
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
#include "../vendor/imgui/imguifilesystem.h"




ButtonLRarrow::ButtonLRarrow(std::string description):
    GUIelement("ButtonLR"),
    description(description),
    spacing(ImGui::GetStyle().ItemInnerSpacing.x)
{


}

void ButtonLRarrow::left(EventManager *eventmanager, Event event)
{
    ImGui::Text(description.c_str());
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton((IMGUIname+"left").c_str(), ImGuiDir_Left)) {
        counter--;
        eventmanager->dispatch(event);
    }
}

void ButtonLRarrow::right(EventManager *eventmanager, Event event)
{
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton((IMGUIname+"right").c_str(), ImGuiDir_Right)) {
        counter++;
        eventmanager->dispatch(event);
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", counter);
}

void MatrixView::showMatrix(mat4& matrix){
        ImGui::Text((IMGUIname+":").c_str());
        static float* f = &matrix[0][0];
        for (int i = 0; i < 4; i++){
                for(int j =0; j <4; j++){
                    ImGui::Text("%.1f", *(f+(4*i+j)));
                    if(j<3) ImGui::SameLine();
                }
            }
}

