#include "meshmodel_sidebar.h"
#include "meshmodel_opengl_scene.h"

void MeshmodelSidebar::SideBarContent()
{
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);

    for(auto& [name,toggle] : meshmodelscene->mode_toggles_)
        kipod::Gui::Checkbox(toggle);

    for(auto model : meshmodelscene->models)
        kipod::Gui::Transform(*model->world_);

    ModelControl();
}

void MeshmodelSidebar::ModelControl(){
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);

    if (ImGui::TreeNode("Load Models")){
        loadPrimitive();
        loadOBJfile();
        ImGui::TreePop();
    }
    if(meshmodelscene->numberOfModels() == 0) return;
//    if(ImGui::TreeNode("Control Model")){
//        focusCamera(scene);
//        rotateModelLocalSpace();
//        translateModelWorldSpace(scene);
//        translateModelLocalSpace(scene);
//        ImGui::TreePop();
//    }
}


void MeshmodelSidebar::loadPrimitive(){
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);

    ImGui::Text("Load Primitive:");
    const char* primitiveChoice[] = { "Cube", "Tetrahedron", "Sphere"};
    static int primitiveChoice_current = 0;
    ImGui::Combo("Type", &primitiveChoice_current, primitiveChoice, IM_ARRAYSIZE(primitiveChoice));
    static int numberPolygons = 3;
    if(primitiveChoice_current==2){
        ImGui::InputInt("input int", &numberPolygons);
    }
    if(ImGui::Button("Add Primitive")){
                            if(primitiveChoice_current==0)      meshmodelscene->loadPrimitive(Cube);
                            else if(primitiveChoice_current==1) meshmodelscene->loadPrimitive(Tetrahedron);
                            else if(primitiveChoice_current==2) meshmodelscene->loadPrimitive(Sphere, std::max(0,numberPolygons));
                            LOG_ENGINE("Loaded Primitive.");
                            meshmodelscene->setActiveModel(meshmodelscene->numberOfModels()-1);
                        }

}

void MeshmodelSidebar::loadOBJfile(){
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);

    ImGui::Text("Load OBJ File:");
                        static bool texturedOption = false;
                        ImGui::Checkbox("Texture?", &texturedOption);
                        const bool browseButtonPressed = ImGui::Button("Add Model");
                        static ImGuiFs::Dialog dlg;
                        const char* chosenPath = dlg.chooseFileDialog(browseButtonPressed);
                        if (strlen(chosenPath)>0) {
                            LOG_ENGINE("Loaded obj model from path {}.",chosenPath);
                            meshmodelscene->loadOBJModel(chosenPath, texturedOption);
                            meshmodelscene->setActiveModel(meshmodelscene->numberOfModels()-1);
                        }

                        if (strlen(dlg.getChosenPath())>0) {
                        ImGui::Text("Chosen file: \"%s\"",dlg.getChosenPath());
                        }

                        static int selectedModel = -1;
                        for (int n = 0; n <  meshmodelscene->numberOfModels(); n++)
                        {
                            char buf[32];
                            sprintf(buf, "Model %d", n);
                            if (ImGui::Selectable(buf, selectedModel == n)){
                                selectedModel = n;
                                meshmodelscene->setActiveModel(selectedModel);
                                }
                        }
                        if(meshmodelscene->numberOfModels())
                        {
                        ImGui::Text("Active Model: \"%i\"",meshmodelscene->activeModel);
                        }
}
