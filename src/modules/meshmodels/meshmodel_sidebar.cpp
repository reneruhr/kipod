#include "meshmodel_sidebar.h"
#include "meshmodel_opengl_scene.h"
#include "../../engine/engine_gui.h"

void MeshmodelSidebar::SideBarContent()
{
    ModelControl();
    CameraControl();
}

void MeshmodelSidebar::ModelControl(){
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);

    LoadPrimitive();
    LoadOBJfile();
    ModelList();
    ModelMoveOptions();
    ModelScaleOptions();
    ModelViewOptions();
}

void MeshmodelSidebar::CameraControl()
{
    CameraList();
    CameraViewOption();
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
    ImGui::PushItemWidth(40);
    ImGui::PushID("Add primitive");
    if(ImGui::Button("Add")){
                            if(primitiveChoice_current==0)      meshmodelscene->LoadPrimitive(Cube);
                            else if(primitiveChoice_current==1) meshmodelscene->LoadPrimitive(Tetrahedron);
                            else if(primitiveChoice_current==2) meshmodelscene->LoadPrimitive(Sphere, std::max(0,numberPolygons));
                            LOG_ENGINE("Loaded Primitive.");
                            meshmodelscene->setActiveModel(meshmodelscene->numberOfModels()-1);
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
        meshmodelscene->setActiveModel(meshmodelscene->numberOfModels()-1);
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
            meshmodelscene->setActiveModel(selectedModel);
            }
    }
   ImGui::Separator();
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
    auto model = meshmodelscene->getActiveModel();
    if(model) {
        ImGui::Text("Move Model");  ImGui::SameLine(); kipod::HelpMarker("Arrow & Page Keys");
        kipod::Gui::Transform(*model->world_);
        ImGui::Separator();
    }
}

void MeshmodelSidebar::ModelScaleOptions()
{
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);
    auto model = meshmodelscene->getActiveModel();
    if(model) {
        kipod::Gui::Scale(*model->local_);
        ImGui::Separator();
    }
}

void MeshmodelSidebar::CameraList()
{
    auto meshmodelscene = static_pointer_cast<MeshModelOpenGLScene>(scene_);
    ImGui::Text("Camera"); ImGui::SameLine();
    if (ImGui::Button("Add##Camera")){
        kipod::RenderCamera* cam = new kipod::RenderCamera({0.0,0.0,3.0});
            meshmodelscene->addCamera(cam);
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
            meshmodelscene->setActiveCamera(selectedCamera);
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
    auto cam = static_pointer_cast<MeshModelOpenGLScene>(scene_)->getActiveCamera();
    camera_projection_toggle.state_ = cam->projection_type_==kipod::RenderCamera::ProjectionType::ORTHOGONAL ? 0 : 1;
    if(setup){
        camera_projection_toggle.delegate_  = {};
        camera_projection_toggle.delegate_.connect<&CameraViewToggle>();
        setup=false;
    }

    kipod::Gui::RadioButtons(camera_projection_toggle, (void*) cam);
    ImGui::Separator();
}



