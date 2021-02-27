#include "shapes_sidebar.h"
#include "shapes_scene.h"
#include "../../engine/engine_gui.h"

void kipod::Shapes::ShapesSidebar::SideBarContent()
{
    AddShape();
    ShapeList();
}

void kipod::Shapes::ShapesSidebar::AddShape()
{
    auto scene = std::static_pointer_cast<ShapesScene>(scene_);
    const char* primitiveChoice[] = { "Square", "Octagon"}; // , "Infinite Mirror"
    static int primitiveChoice_current = 0;

    ImGui::PushItemWidth(70);
    ImGui::Combo("##Type", &primitiveChoice_current, primitiveChoice, IM_ARRAYSIZE(primitiveChoice));
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PushID("Add primitive");
    if(ImGui::Button("Add")){
                            if(primitiveChoice_current==0)      scene->AddShape(Shape(Square()));
                            else if(primitiveChoice_current==1) scene->AddShape(Shape(Octagon()));
                            //else if(primitiveChoice_current==2)  scene->mirror_ = scene->framebuffer_->tex_->Square();
                            LOG_ENGINE("Loaded Shape.");
                            LOG_CONSOLE("Loaded Shape.");
                        }

    ImGui::PopID();
    ImGui::Separator();
}

void kipod::Shapes::ShapesSidebar::ShapeList()
{
    auto scene = std::static_pointer_cast<ShapesScene>(scene_);
    auto& shapes = scene->shapes_;
    static int selected_shape = 0;
    if(scene->HasShape()){
        auto current_shape = std::begin(shapes);
        for (int n = 0; n <  (int)shapes.size(); n++, ++current_shape)
        {
            char buf[32];
            sprintf(buf, "Shape %d", n);
            if (ImGui::Selectable(buf, selected_shape == n)){
                selected_shape = n;
                scene->ActiveShape(current_shape->get());
                }
            ImGui::SameLine();
            float* color = &current_shape->get()->mat_->emission_[0];
            ImGui::PushID(n);
            ImGui::ColorEdit4("ShapeColor#", color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
            ImGui::PopID();
            ImGui::SameLine();
            ImGui::Text("%.1f", current_shape->get()->depth_);
        }
        ImGui::Separator();
    }
}
