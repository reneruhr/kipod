#include "meshmodel_sidebar.h"
#include "meshmodel_opengl_scene.h"
#include "../../engine/engine_gui.h"

void MeshmodelSidebar::SideBarContent()
{
    Help();
    if(kipod::Gui::TreeNode("Models")){
        ModelControl();
        kipod::Gui::TreePop();}
    if(kipod::Gui::TreeNode("Lights")){
        LightControl();
        kipod::Gui::TreePop();}
    if(kipod::Gui::TreeNode("Camera")){
        CameraControl();
        kipod::Gui::TreePop();}
}

void MeshmodelSidebar::Help()
{
    kipod::HoverInfo("Controls", "Keyboard controls only work if the 'Viewport' window is active. "
                      "Click inside to active it.\n"
                      "Mouse Control View: Hold left mouse button and move mouse.\n"
                      "Camera: ASDFRF Keys. Optional: Hold CTRL.\n"
                      "Model: Arrow & Page Down/Up Keys.\n"
                      "It is possible to drag windows around.\n");
}

void MeshmodelSidebar::ModelControl(){
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);

    LoadPrimitive();
    LoadOBJfile();
    ModelList();
    ModelMoveOptions();
    ModelScaleOptions();
    ModelMaterialOptions();
    ModelViewOptions();
}

void MeshmodelSidebar::CameraControl()
{
    CameraList();
    CameraViewOption();
}


void MeshmodelSidebar::LightControl()
{

    LightOptions();
    LightAdd();
    LightViewOptions();

}

void MeshmodelSidebar::LightViewOptions()
{
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);

    kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Colors"]);
    kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Textures"]);
    kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Emissive"]);

    ImGui::Separator();
}

void MeshmodelSidebar::LightOptions()
{
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);
    if(meshmodelscene->HasLight()){
        static unsigned int selectedLight = 0;
        for (unsigned int n = 0; n <  meshmodelscene->lights.size(); n++)
        {
            char buf[32];
            sprintf(buf, "Light %d", n);
            if (ImGui::Selectable(buf, selectedLight == n)) selectedLight = n;
        }
        kipod::RenderLight* light = meshmodelscene->lights[selectedLight];
        if(light->Type() == kipod::LightSource::AMBIENT) ImGui::Text("Ambient");
        else if(light->Type() == kipod::LightSource::DIFFUSE) ImGui::Text("Diffuse");
        else if(light->Type() == kipod::LightSource::SPECULAR) ImGui::Text("Specular");

        glm::vec4 lightSourceLocationLocal = light->Source();
        glm::vec4 lightColor = light->Color();
        ImGui::ColorEdit4("##LightColorChoice2", (float*)&lightColor, 0);
        ImGui::SliderFloat3("##Source Location2", &lightSourceLocationLocal[0], -10.0f, 10.0f);
        light->Color() = lightColor;
        light->Source() = lightSourceLocationLocal;
    }
    ImGui::Separator();
}

void MeshmodelSidebar::LightAdd()
{
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);

    static ImVec4 clight = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f);
    const char* lightChoices[] = { "Ambient", "Diffuse", "Specular"};
    static glm::vec4 lightSourceLocation = {0,0,0,1};
    static int lightChoice_current = 0;

    ImGui::SetNextItemWidth(70.0f);
    ImGui::Combo("##AddType", &lightChoice_current, lightChoices, IM_ARRAYSIZE(lightChoices));
    ImGui::SameLine();
    if (ImGui::Button("Add##Light")){
            static kipod::LightSource source;
            if(lightChoice_current==0) source = kipod::LightSource::AMBIENT;
            else if(lightChoice_current==1) source = kipod::LightSource::DIFFUSE;
            else if(lightChoice_current==2) source = kipod::LightSource::SPECULAR;
            kipod::RenderLight* light =
            new kipod::RenderLight(source,
                      glm::vec4(lightSourceLocation),
                      glm::vec4(float(clight.x),float(clight.y),float(clight.z),float(clight.w)));
            meshmodelscene->AddLight(light);
    }
    ImGui::ColorEdit4("##LightAddColor", (float*)&clight, 0);
    if(lightChoice_current) {
        ImGui::SliderFloat3("##newlightpos", &lightSourceLocation[0], -10.0f, 10.0f);
    }
    ImGui::Separator();

}


void MeshmodelSidebar::LoadPrimitive(){
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);

    const char* primitiveChoice[] = { "Cube", "Tetrahedron", "Sphere"};
    static int primitiveChoice_current = 0;
    static int numberPolygons = 3;

    ImGui::PushItemWidth(70);
    ImGui::Combo("##Type", &primitiveChoice_current, primitiveChoice, IM_ARRAYSIZE(primitiveChoice));
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PushID("Add primitive");
    if(ImGui::Button("Add")){
                            if(primitiveChoice_current==0)      meshmodelscene->LoadPrimitive(Cube);
                            else if(primitiveChoice_current==1) meshmodelscene->LoadPrimitive(Tetrahedron);
                            else if(primitiveChoice_current==2) meshmodelscene->LoadPrimitive(Sphere, std::max(0,numberPolygons));
                            LOG_ENGINE("Loaded Primitive.");
                            meshmodelscene->SetActiveModel(meshmodelscene->numberOfModels()-1);
                        }

    if(primitiveChoice_current==2){
        ImGui::PushItemWidth(70);
        ImGui::InputInt("# Subdivisions", &numberPolygons);
        ImGui::PopItemWidth();
    }
    ImGui::PopID();
    ImGui::Separator();
}

void MeshmodelSidebar::LoadOBJfile(){
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);

    static bool texturedOption = true;
    static ImGuiFs::Dialog dlg;

    ImGui::Text("Obj Model     ");
    ImGui::SameLine();
    ImGui::PushID("Add Obj Model");
    const bool browseButtonPressed = ImGui::Button("Add");
    ImGui::Checkbox("Add Texture", &texturedOption);
    ImGui::PopID();
    const char* chosenPath = dlg.chooseFileDialog(browseButtonPressed);
    if (strlen(chosenPath)>0) {
        LOG_ENGINE("Loaded obj model from path {}.",chosenPath);
        LOG_CONSOLE("Chosen file: \"%s\"",chosenPath);
        meshmodelscene->LoadOBJModel(chosenPath, texturedOption);
        meshmodelscene->SetActiveModel(meshmodelscene->numberOfModels()-1);
    }
     ImGui::Separator();
}

void MeshmodelSidebar::ModelList()
{
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);
    static int selectedModel = meshmodelscene->activeModel;
    for (int n = 0; n <  meshmodelscene->numberOfModels(); n++)
    {
        char buf[32];
        sprintf(buf, "Model %d", n);
        if (ImGui::Selectable(buf, selectedModel == n)){
            selectedModel = n;
            meshmodelscene->SetActiveModel(selectedModel);
            }
    }
   if(meshmodelscene->HasModel()) ImGui::Separator();
}

void MeshmodelSidebar::ModelViewOptions()
{
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);

    kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Normals"]);
    ImGui::SameLine(); kipod::HelpMarker("N Key");
    kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Wireframe"]);
    ImGui::SameLine(); kipod::HelpMarker("Space Key");
    kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Bounding Box"]);
    ImGui::SameLine(); kipod::HelpMarker("B Key");

    ImGui::Separator();
}

void MeshmodelSidebar::ModelMoveOptions(){
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);
    auto model = meshmodelscene->GetActiveModel();
    if(model) {
        ImGui::Text("Move Model");  ImGui::SameLine(); kipod::HelpMarker("Arrow & Page Keys");
        kipod::Gui::Transform(*model->world_);
        ImGui::Separator();
    }
}

void MeshmodelSidebar::ModelScaleOptions()
{
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);
    auto model = meshmodelscene->GetActiveModel();
    if(model) {
        kipod::Gui::Scale(*model->local_);
        ImGui::Separator();
    }
}

void MeshmodelSidebar::ModelMaterialOptions()
{
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);
    auto model = meshmodelscene->GetActiveModel();
    if(model) {
        kipod::Gui::Color(*model->mat_);
        ImGui::Separator();
    }
}

void MeshmodelSidebar::CameraList()
{
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);
    ImGui::Text("Camera"); ImGui::SameLine();
    if (ImGui::Button("Add##Camera")){
        kipod::RenderCamera* cam = new kipod::RenderCamera({0.0,0.0,3.0});
            meshmodelscene->AddCamera(cam);
    }
    ImGui::SameLine(); kipod::HelpMarker("Camera Movement:\n"
                                         "ASWDRF Keys for movement w.r.t. looking direction. Changes focus.\n"
                                         "CTRL + ASWDRF Keys for movement along x,y,z Axis. Keeps focus fixed.\n"
                                         );

    static int selectedCamera = -1;
    for (int n = 0; n <  meshmodelscene->numberOfCameras(); n++)
    {
        char buf[32];
        sprintf(buf, "Camera %d", n);
        if (ImGui::Selectable(buf, selectedCamera == n)){
            selectedCamera = n;
            meshmodelscene->SetActiveCamera(selectedCamera);
        }
    }

    ImGui::Separator();
}

void CameraViewToggle(void* ptr, int i){
    auto cam = static_cast<kipod::RenderCamera*>(ptr);
    if(i==0) {
        cam->MakeProjection(kipod::RenderCamera::ProjectionType::ORTHOGONAL);
        cam->projection_type_ =   kipod::RenderCamera::ProjectionType::ORTHOGONAL;
    }
    if(i==1){
        cam->MakeProjection(kipod::RenderCamera::ProjectionType::PROJECTIVE);
        cam->projection_type_ =   kipod::RenderCamera::ProjectionType::PROJECTIVE;
    }
}

void MeshmodelSidebar::CameraViewOption()
{
    static bool setup = true;
    static kipod::MultipleModeToggle camera_projection_toggle({"Orthographic", "Projective"});
    auto cam = static_pointer_cast<MeshModelOpenGLScene>(scene_)->GetActiveCamera();
    camera_projection_toggle.state_ = cam->projection_type_==kipod::RenderCamera::ProjectionType::ORTHOGONAL ? 0 : 1;
    if(setup){
        camera_projection_toggle.delegate_  = {};
        camera_projection_toggle.delegate_.connect<&CameraViewToggle>();
        setup=false;
    }

    kipod::Gui::RadioButtons(camera_projection_toggle, (void*) cam);
    ImGui::Separator();
}




