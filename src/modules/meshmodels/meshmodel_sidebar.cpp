#include "meshmodel_sidebar.h"
#include "meshmodel_scene.h"
#include "../../engine/engine_gui.h"

namespace kipod::MeshModels{

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
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);

    LoadPrimitive();
    LoadQuadricPrimitive();
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
    CameraProjectiveOption();
}


void MeshmodelSidebar::LightControl()
{

    LightOptions();
    LightAdd();
    LightViewOptions();

}

void MeshmodelSidebar::LightViewOptions()
{
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);

    kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Colors"]);
    kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Textures"]);
    kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Emissive"]);

    ImGui::Separator();
}

void MeshmodelSidebar::LightOptions()
{
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);
    if(meshmodelscene->HasLight()){
        static unsigned int selectedLight = 0;
        for (unsigned int n = 0; n <  meshmodelscene->lights_.size(); n++)
        {
            char buf[32];
            if(meshmodelscene->lights_[n]->Type() == kipod::LightSource::AMBIENT) sprintf(buf, "Light %d %s", n, "Ambient");
            else if(meshmodelscene->lights_[n]->Type() == kipod::LightSource::DIFFUSE) sprintf(buf, "Light %d %s", n, "Diffuse");
            else if(meshmodelscene->lights_[n]->Type() == kipod::LightSource::SPECULAR) sprintf(buf, "Light %d %s", n, "Specular");
            if (ImGui::Selectable(buf, selectedLight == n)) selectedLight = n;
        }
        kipod::RenderLight* light = meshmodelscene->lights_[selectedLight].get();
        ImGui::Text("Modify: "); ImGui::SameLine();
        if(light->Type() == kipod::LightSource::AMBIENT) ImGui::Text("Ambient");
        else if(light->Type() == kipod::LightSource::DIFFUSE) ImGui::Text("Diffuse");
        else if(light->Type() == kipod::LightSource::SPECULAR) ImGui::Text("Specular");

        glm::vec4 lightSourceLocationLocal = light->Source();
        glm::vec4 lightColor = light->Color();
        if(ImGui::ColorEdit4("##LightColorChoice2", (float*)&lightColor, 0))
            meshmodelscene->NeedsUpdate();
        if(ImGui::SliderFloat3("##Source Location2", &lightSourceLocationLocal[0], -100.0f, 100.0f))
            meshmodelscene->NeedsUpdate();
        light->Color() = lightColor;
        light->Source() = lightSourceLocationLocal;

        if(kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Show Lights"]))
                meshmodelscene->NeedsUpdate();
    }
    ImGui::Separator();
}

void MeshmodelSidebar::LightAdd()
{
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);

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
            meshmodelscene->AddLight(std::move(*light));
    }
    ImGui::ColorEdit4("##LightAddColor", (float*)&clight, 0);
    if(lightChoice_current) {
        ImGui::SliderFloat3("##newlightpos", &lightSourceLocation[0], -10.0f, 10.0f);
    }
    ImGui::Separator();

}


void MeshmodelSidebar::LoadPrimitive(){
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);

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
                            meshmodelscene->SetActiveModel(meshmodelscene->NumberOfModels()-1);
                        }

    if(primitiveChoice_current==2){
        ImGui::PushItemWidth(70);
        ImGui::InputInt("# Subdivisions", &numberPolygons);
        ImGui::PopItemWidth();
    }
    ImGui::PopID();
    ImGui::Separator();
}

void MeshmodelSidebar::LoadQuadricPrimitive()
{
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);

    const char* quadricChoice[] = { "Sphere", "Ellipsoid"};
    static int quadricChoice_current = 0;
    static float quadric_a=1, quadric_b=1, quadric_c=1;

    ImGui::PushItemWidth(70);
    ImGui::Combo("##Type_primitive", &quadricChoice_current, quadricChoice, IM_ARRAYSIZE(quadricChoice));
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::PushID("Add Quadric");
    if(ImGui::Button("Add")){
                            if(quadricChoice_current==0){
                                meshmodelscene->LoadPrimitive(Quadric::Sphere);
                                auto sphere = dynamic_cast<PrimMeshModel*>(meshmodelscene->render_objects_.back().get())->GetRaytracingQuadric();
                                sphere->Q_[3][3] = quadric_a;
                            }
                            else if(quadricChoice_current==1) {
                                meshmodelscene->LoadPrimitive(Quadric::Ellipsoid);
                                auto ellipsoid = dynamic_cast<PrimMeshModel*>(meshmodelscene->render_objects_.back().get())->GetRaytracingQuadric();
                                auto& Q = ellipsoid->Q_;
                                Q[0][0] = quadric_a;
                                Q[1][1] = quadric_b;
                                Q[2][2] = quadric_c;
                            }
                            LOG_ENGINE("Loaded Quadric Primitive.");
                            meshmodelscene->SetActiveModel(meshmodelscene->NumberOfModels()-1);
    }

    if(quadricChoice_current==0){
        ImGui::PushItemWidth(70);
        ImGui::InputFloat("radius", &quadric_a);
        ImGui::PopItemWidth();
    }
    if(quadricChoice_current==1){
        ImGui::PushItemWidth(70);
        ImGui::InputFloat("a", &quadric_a);
        ImGui::InputFloat("b", &quadric_b);
        ImGui::InputFloat("c", &quadric_c);
        ImGui::PopItemWidth();
    }
    ImGui::PopID();
    ImGui::Separator();
}

void MeshmodelSidebar::LoadOBJfile(){
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);

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

        auto path = std::filesystem::path(chosenPath);
        if(path.extension() != ".obj") {
            LOG_ENGINE("Not an OBJ file.");
            LOG_CONSOLE("Not an OBJ file.");
            return;
        }
        LOG_ENGINE("Loaded obj model from path {}.",chosenPath);
        LOG_CONSOLE("Chosen file: \"%s\"",chosenPath);
        meshmodelscene->LoadOBJModel(chosenPath, texturedOption);
        meshmodelscene->SetActiveModel(meshmodelscene->NumberOfModels()-1);
    }
     ImGui::Separator();
}

void MeshmodelSidebar::ModelList()
{
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);
    static int selectedModel = 0;
    for (int n = 0; n <  meshmodelscene->NumberOfModels(); n++)
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
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);

    if(kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Normals"]))
        meshmodelscene->NeedsUpdate();
    ImGui::SameLine(); kipod::HelpMarker("N Key");
    if(auto model = meshmodelscene->GetActiveModel()){
        ImGui::SameLine();
        ImGui::SetNextItemWidth(70.0f);
        ImGui::InputFloat("##normal_length", &model->normal_length, 0.01f,10.0f, "%.1f");
    }
    if(kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Wireframe"]))
        meshmodelscene->NeedsUpdate();
    ImGui::SameLine(); kipod::HelpMarker("Space Key");
    if(kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Bounding Box"]))
        meshmodelscene->NeedsUpdate();
    ImGui::SameLine(); kipod::HelpMarker("B Key");

    ImGui::Separator();
}

void MeshmodelSidebar::ModelMoveOptions(){
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);
    auto model = meshmodelscene->GetActiveModel();
    if(model) {
        ImGui::Text("Move Model");  ImGui::SameLine(); kipod::HelpMarker("Arrow & Page Keys");
        if(kipod::Gui::Transform(*model->world_))
            meshmodelscene->NeedsUpdate();
        ImGui::Separator();
    }
}

void MeshmodelSidebar::ModelScaleOptions()
{
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);
    auto model = meshmodelscene->GetActiveModel();
    if(model) {
        if(kipod::Gui::Scale(*model->local_))
            meshmodelscene->NeedsUpdate();
        ImGui::Separator();
    }
}

void MeshmodelSidebar::ModelMaterialOptions()
{
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);
    auto model = meshmodelscene->GetActiveModel();
    if(model) {
        if(kipod::Gui::Color(*model->mat_))
            meshmodelscene->NeedsUpdate();
        ImGui::Separator();
    }
}

void MeshmodelSidebar::CameraList()
{
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);
    ImGui::Text("Camera"); ImGui::SameLine();
    if (ImGui::Button("Add##Camera")){
        meshmodelscene->AddCamera({{0.0,0.0,3.0}});
    }
    ImGui::SameLine(); kipod::HelpMarker("Camera Movement:\n"
                                         "ASWDRF Keys for movement w.r.t. looking direction. Changes focus.\n"
                                         "CTRL + ASWDRF Keys for movement along x,y,z Axis. Keeps focus fixed.\n"
                                         );

    static int selectedCamera = -1;
    for (int n = 0; n <  meshmodelscene->NumberOfCameras(); n++)
    {
        char buf[32];
        sprintf(buf, "Camera %d", n);
        if (ImGui::Selectable(buf, selectedCamera == n)){
            selectedCamera = n;
            meshmodelscene->SetActiveCamera(selectedCamera);
            meshmodelscene->NeedsUpdate();
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
    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);
    static bool setup = true;
    static kipod::MultipleModeToggle camera_projection_toggle({"Orthographic", "Projective"});
    auto cam = std::static_pointer_cast<MeshModelScene>(scene_)->GetActiveCamera();
    camera_projection_toggle.state_ = cam->projection_type_==kipod::RenderCamera::ProjectionType::ORTHOGONAL ? 0 : 1;
    if(setup){
        camera_projection_toggle.delegate_  = {};
        camera_projection_toggle.delegate_.connect<&CameraViewToggle>();
        setup=false;
    }

    if(kipod::Gui::RadioButtons(camera_projection_toggle, (void*) cam))
        meshmodelscene->NeedsUpdate();

    ImGui::InputFloat3("##camerapos", &cam->Eye()[0], "%.1f");
    ImGui::SameLine(); ImGui::Text("Eye");
    if(kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Show Cameras"]))
            meshmodelscene->NeedsUpdate();
    if(kipod::Gui::Checkbox(meshmodelscene->mode_toggles_["Show Frustum"]))
            meshmodelscene->NeedsUpdate();
    ImGui::Separator();
}

void MeshmodelSidebar::CameraProjectiveOption()
{
    static RenderCamera* cam = std::static_pointer_cast<MeshModelScene>(scene_)->GetActiveCamera();

    auto meshmodelscene = std::static_pointer_cast<MeshModelScene>(scene_);
    static int selectedCamera = -1;
    for (int n = 0; n <  meshmodelscene->NumberOfCameras(); n++)
    {
        char buf[32];
        sprintf(buf, "Camera %d ##Modify", n);
        if (ImGui::Selectable(buf, selectedCamera == n)){
            selectedCamera = n;
            cam = meshmodelscene->GetCamera(selectedCamera);
        }
    }
    ImGui::Separator();

    if(cam->projection_type_==kipod::RenderCamera::ProjectionType::PROJECTIVE)
    {
        static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
        static float slider_fovy = cam->fovy_;
        static float slider_aspect_ratio = cam->aspect_;
        static float slider_near = cam->near_;
        static float slider_far = cam->far_;
        static bool changed_perspective = false;

        if(ImGui::SliderFloat("FOVY (35 -> 55)", &slider_fovy, 35.0f, 55.0f, "%.0f", flags)) changed_perspective = true;
        if(ImGui::SliderFloat("Aspect ratio (0.1 -> 10)", &slider_aspect_ratio, 0.1f, 10.0f, "%.1f", flags)) changed_perspective = true;
        if(ImGui::SliderFloat("Near (0.1 -> 10.0)", &slider_near, 0.1f, 10.0f, "%.1f", flags)) changed_perspective = true;
        if(ImGui::SliderFloat("Far (10 -> 1000)", &slider_far, 10.0f, 1000.0f, "%.0f", flags)) changed_perspective = true;

        if(changed_perspective){
            cam->ChangePerspective(slider_fovy, slider_aspect_ratio, slider_near, slider_far);
            changed_perspective = false;
            meshmodelscene->NeedsUpdate();
        }
    }
}




}
