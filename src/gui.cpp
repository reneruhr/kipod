#include "gui.h"

#include "guielement.h"
#include "utils/log.h"




GUI::~GUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::init(kipod::Window *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();  (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    io.WantCaptureKeyboard=1;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->_window, true);
    ImGui_ImplOpenGL3_Init(window->glsl_version);



    // FROM HAZEL:

    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

}


void GUI::Begin(kipod::Window* window){

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    ImGuiIO &io = ImGui::GetIO();
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    static bool opt_fullscreen = true;
    static bool dockspace = true;


    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspace, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }


    if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::Separator();


                if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::Separator();

//                if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
//                    *p_open = false;
                ImGui::EndMenu();
            }
//            HelpMarker(
//                "When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n\n"
//                " > if io.ConfigDockingWithShift==false (default):" "\n"
//                "   drag windows from title bar to dock" "\n"
//                " > if io.ConfigDockingWithShift==true:" "\n"
//                "   drag windows from anywhere and hold Shift to dock" "\n\n"
//                "This demo app has nothing to do with it!" "\n\n"
//                "This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window. This is useful so you can decorate your main application window (e.g. with a menu bar)." "\n\n"
//                "ImGui::DockSpace() comes with one hard constraint: it needs to be submitted _before_ any window which may be docked into it. Therefore, if you use a dock spot as the central point of your application, you'll probably want it to be part of the very first window you are submitting to imgui every frame." "\n\n"
//                "(NB: because of this constraint, the implicit \"Debug\" window can not be docked into an explicit DockSpace() node, because that window is submitted as part of the NewFrame() call. An easy workaround is that you can create your own implicit \"Debug##2\" window after calling DockSpace() and leave it in the window stack for anyone to use.)"
//            );

            ImGui::EndMenuBar();
        }


}

void GUI::Draw(Scene* scene, SoftRenderer* softrenderer, kipod::Window* window)
{

    ImGui::Begin("Modules");
    draw_menus(scene, softrenderer, window);

    for(auto m : gui_modules_) m->Draw();
    ImGui::End();


//    ImGui::Begin("Quasicrystal");

//    unsigned int scene_texture = scene->SceneAsFramebuffer();
//    ImVec2 viewport_size = ImGui::GetContentRegionAvail();
//    unsigned int x = static_cast<unsigned int>(viewport_size.x*1.2);
//    unsigned int y = static_cast<unsigned int>(viewport_size.y*1.2);
//    if(scene->width_ != x || scene->height_ != y)
//        scene->Resize(x, y);
//    ImGui::Image(reinterpret_cast<void*>(scene_texture), viewport_size, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

//    ImGui::End();



}

void GUI::End(kipod::Window* window)
{

    ImGui::Begin("ImGUI Demo");
    static bool show_demo_ = false;
    if(ImGui::Button("ShowDemoWindow"))    show_demo_ = !show_demo_;
    if(show_demo_) ImGui::ShowDemoWindow();
    ImGui::End();
    ImGui::End();


    ImGuiIO &io = ImGui::GetIO();

    io.DisplaySize = ImVec2((float)window->Width(), (float)window->Height());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

}


void GUI::drawYotamBirthday(Scene* scene){
    if(ImGui::Button("Mazal Tov")){
        const char* path = "shaders/extra/Teddy.obj";
        scene->LoadOBJModel(path);
//        scene->initLastModel();
        scene->setActiveModel(scene->numberOfModels()-1);
        //scene->moveModel(scene->numberOfModels()-1, vec3(0,0,1)  );
        scene->activeCamera =0;
        eventmanager->dispatch( Event(EventType::RotateModel, EventData(RotateDirection::YAW, 180.0)) );
    }
}


void GUI::draw_menus(Scene* scene, SoftRenderer* softrenderer, kipod::Window* window)
{
    drawYotamBirthday(scene);
    if (ImGui::CollapsingHeader("Renderer")){
        drawSoftRendererControl(scene, softrenderer, window);
        drawModelControl(scene);
        drawCameraControl(scene);

        drawMaterialsControl(scene);
        //drawTextureControl(scene);
        drawLightControl(scene);
        drawNormalControl(scene);
        drawBBOXControl(scene);

        drawPointSetControl(scene);
    }

}

void GUI::drawSoftRendererControl(Scene* scene, SoftRenderer* softrenderer, kipod::Window* window){
    if (ImGui::TreeNode("Software Renderer"))
        {
        softRenderScene(scene, window);
        selectLineAlgorithm(softrenderer);
        ImGui::TreePop();
    }
}

void GUI::drawMaterialsControl(Scene *scene)
{
    if(scene->numberOfModels() == 0) return;
    if (ImGui::TreeNode("Materials")){
        showMaterials(scene);
        if (ImGui::TreeNode("AddMaterials"))
            {
            addMaterials(scene);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}

void GUI::drawLightControl(Scene *scene)
{
    if (ImGui::TreeNode("Lights"))
        {
        lightControl(scene);
        ImGui::TreePop();
    }
}

void GUI::drawPointSetControl(Scene *scene)
{
    if (ImGui::TreeNode("PointSets")){
        static MatrixView basisView("Basis");

        if(scene->HasPointSet()){
            PointSet* point_set = scene->point_sets_.back();
            basisView.showMatrix(point_set->basis_);
        }
        ImGui::TreePop();
    }
}

void GUI::drawNormalControl(Scene* scene){
    if(ImGui::TreeNode("Normals")){
     drawNormals(scene);
     ImGui::TreePop();
    }
}

void GUI::drawBBOXControl(Scene* scene){
    if(ImGui::TreeNode("Bounding Box")){
     drawBoundingBox(scene);
     ImGui::TreePop();
    }
}

void GUI::drawModelControl(Scene* scene){

    if (ImGui::TreeNode("Load Models")){
            LoadPrimitive(scene);
            loadOBJfile(scene);
        ImGui::TreePop();
    }
    if(scene->numberOfModels() == 0) return;
    if(ImGui::TreeNode("Control Model")){
//        focusCamera(scene);
//        rotateModelLocalSpace();
//        translateModelWorldSpace(scene);
//        translateModelLocalSpace(scene);
        ImGui::TreePop();
    }
}

void GUI::drawCameraControl(Scene *scene)
{
    if (ImGui::TreeNode("Cameras")){
        Camera* cam = scene->getActiveCamera();

        if (ImGui::TreeNode("List of Camera")){
            add_and_list_cameras(scene);
            ImGui::TreePop();
            }
        if(ImGui::TreeNode("Draw Frustum:")){
            frustum_camera(scene);
            ImGui::TreePop();
            }
        if(ImGui::TreeNode("Perspective Matrix:")){
            toggle_view(scene);
            perspective_camera(cam);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Projective Matrix:")){
            toggle_view(scene);
            projective_camera(cam);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Control Camera:")){
            control_camera(cam);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Draw Camera:")){
            drawCameras(scene);
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}




void GUI::selectLineAlgorithm(SoftRenderer* softrenderer){
    static int selectedlineAlgo = 0;
    for (int n = 0; n < 4; n++)
    {
        char buf[32];
        sprintf(buf, "Line Algo %d", n);
        if (ImGui::Selectable(buf, selectedlineAlgo == n)){
            selectedlineAlgo = n;
            switch(n){
                case(0):  softrenderer->lineAlgo=DDA; break;
                case(1):  softrenderer->lineAlgo=MIDPOINT_SLOW; break;
                case(2):  softrenderer->lineAlgo=BREZENHAM; break;
                case(3):  softrenderer->lineAlgo=BREZENHAM_WIKI; break;
            }
         }
    }
}

void GUI::softRenderScene(Scene* scene, kipod::Window* window){
    if(ImGui::Button("Activate CG Course Buffer")){
        scene->cg_active = !scene->cg_active;
        window->SplitScreen(scene->cg_active);

    }
    if(ImGui::Button("Clear CG Buffer:")){
        scene->clearBuffer();
    }
    if(ImGui::Button("Draw Demo")){
        scene->drawDemo();
    }
}

void GUI::addMaterials(Scene *scene)
{
//    MeshModel* model = scene->getActiveModel();
//    static ImVec4 colorAmbient = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
//    static ImVec4 colorDiffuse = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
//    static ImVec4 colorSpecular = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
//    static ImVec4 colorEmission = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
//    static float slider_shininess = 1.0f;
//    ImGui::Text("Ambient Color");
//    ImGui::ColorEdit3("MyColor##1", (float*)&colorAmbient, 0);
//    ImGui::Text("Diffuse Color");
//    ImGui::ColorEdit3("MyColor##2", (float*)&colorDiffuse, 0);
//    ImGui::Text("Specular Color");
//    ImGui::ColorEdit3("MyColor##3", (float*)&colorSpecular, 0);
//    ImGui::Text("Emission Color");
//    ImGui::ColorEdit3("MyColor##4", (float*)&colorEmission, 0);
//    ImGui::SliderFloat("Shininess", &slider_shininess, 0.0f, 500.0f, "%.3f", ImGuiSliderFlags_None);

//    if (ImGui::Button("Add Uniform Material")){
//        MaterialStruct material = {
//            vec4(colorAmbient.x,colorAmbient.y,colorAmbient.z,colorAmbient.w),
//            vec4(colorDiffuse.x,colorDiffuse.y,colorDiffuse.z,colorDiffuse.w),
//            vec4(colorSpecular.x,colorSpecular.y,colorSpecular.z,colorSpecular.w),
//            vec4(colorEmission.x,colorEmission.y,colorEmission.z,colorEmission.w),
//            float(slider_shininess)
//        };
//        model->SetUniformMaterial(material);
//    }
}


void GUI::showMaterials(Scene *scene)
{
//    MeshModel* model = scene->getActiveModel();
//    if(!model->colors_vector.empty()){
//        static unsigned int selectedMaterial = 0;
//        for (unsigned int n = 0; n <  model->colors_vector.size(); n++)
//        {
//            char buf[32];
//            sprintf(buf, "Material %d", n);
//            if (ImGui::Selectable(buf, selectedMaterial == n)) selectedMaterial = n;
//        }
//        MaterialStruct material = model->colors_vector[selectedMaterial];
//        ImGui::Text("Ambient Color");
//        if(ImGui::ColorEdit3("MaterialControl##1", (float*)&material.ambient, 0)) scene->needs_update = true;
//        ImGui::Text("Diffuse Color");
//        if(ImGui::ColorEdit3("MaterialControl##2", (float*)&material.diffuse, 0)) scene->needs_update = true;
//        ImGui::Text("Specular Color");
//        if(ImGui::ColorEdit3("MaterialControl##3", (float*)&material.specular, 0)) scene->needs_update = true;
//        ImGui::Text("Emission Color");
//        if(ImGui::ColorEdit3("MaterialControl##4", (float*)&material.emission, 0)) scene->needs_update = true;
//        if(ImGui::SliderFloat("Shininess", &material.shininess, 0.0f, 500.0f, "%.3f", ImGuiSliderFlags_None)) scene->needs_update = true;
//        model->colors_vector[selectedMaterial] = material;
//    }

}



void GUI::lightControl(Scene *scene)
{
    static bool checkEmmisive = false;
    if(ImGui::Checkbox("Emmisive Light", &checkEmmisive))
                    eventmanager->dispatch(Event(EventType::EmissiveMode, Mode::SWITCH));
    static bool checkLight = true;
    if(ImGui::Checkbox("Lights on", &checkLight))
        eventmanager->dispatch(Event(EventType::LightMode, Mode::SWITCH));
    static bool wireFrameCheck = false;
    if(ImGui::Checkbox("WireFrame Mode", &wireFrameCheck)){
        eventmanager->dispatch(Event(EventType::WireframeMode, Mode::SWITCH));
    }
    static bool textureCheck = false;
    if(ImGui::Checkbox("Texture Mode", &textureCheck)){
        eventmanager->dispatch(Event(EventType::TextureMode, Mode::SWITCH));
    }



    checkEmmisive= scene->emissive_mode;
    checkLight= scene->color_mode;
    wireFrameCheck = scene ->wireframemode;
    textureCheck = scene->texture_mode;



    ImGui::Text("Choose Light:");
    static ImVec4 clight = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f);
    ImGui::ColorEdit3("LightColorChoice##1", (float*)&clight, 0);
    const char* lightChoices[] = { "Ambient", "Diffuse", "Specular"};
    static int lightChoice_current = 0;
    ImGui::Combo("Light Type", &lightChoice_current, lightChoices, IM_ARRAYSIZE(lightChoices));
    static vec4 lightSourceLocation = vec4(0,0,0,1);
    if(lightChoice_current) {
        ImGui::SliderFloat3("Source Location", &lightSourceLocation[0], -10.0f, 10.0f);
    }

    if (ImGui::Button("Add Light")){
            static kipod::LightSource source;
            if(lightChoice_current==0) source = kipod::LightSource::AMBIENT;
            else if(lightChoice_current==1) source = kipod::LightSource::DIFFUSE;
            else if(lightChoice_current==2) source = kipod::LightSource::SPECULAR;
            Light* light =
            new Light(source,
                      vec4(lightSourceLocation),
                      vec4(float(clight.x),float(clight.y),float(clight.z),float(clight.w)));
            scene->addLight(light);
    }

    if(scene->hasLight()){
        static unsigned int selectedLight = 0;
        for (unsigned int n = 0; n <  scene->lights.size(); n++)
        {
            char buf[32];
            sprintf(buf, "Light %d", n);
            if (ImGui::Selectable(buf, selectedLight == n)) selectedLight = n;
        }
        Light* light = scene->lights[selectedLight];
        if(light->getType() == kipod::LightSource::AMBIENT) ImGui::Text("Ambient");
        else if(light->getType() == kipod::LightSource::DIFFUSE) ImGui::Text("Diffuse");
        else if(light->getType() == kipod::LightSource::SPECULAR) ImGui::Text("Specular");

        vec4 lightSourceLocationLocal = light->getSource();
        vec4 lightColor = light->getColor();
        if(ImGui::ColorEdit3("LightColorChoice##2", (float*)&lightColor, 0)) scene->needs_update = true;
        if(ImGui::SliderFloat3("Source Location##2", &lightSourceLocationLocal[0], -10.0f, 10.0f)) scene->needs_update = true;
        light->color = lightColor;
        light->source = lightSourceLocationLocal;
    }
}



void GUI::drawCameras(Scene *scene){
    ImGui::Text("Draw Cameras");
                        if(ImGui::Button("Cameras on/off")){
                            scene->camera_mode=!scene->camera_mode;
                        }
                        ImGui::SameLine();
                        ImGui::Text("%i", scene->camera_mode);
}

void GUI::add_and_list_cameras(Scene *scene)
{

            if (ImGui::Button("Add Camera")){
                    Camera* cam = new Camera( 45, 800.0/600.0, 0.1, 100.0);
                    scene->addCamera(cam);
            }

            static int selectedCamera = -1;
            for (int n = 0; n <  scene->numberOfCameras(); n++)
            {
                char buf[32];
                sprintf(buf, "Camera %d", n);
                if (ImGui::Selectable(buf, selectedCamera == n)){
                    selectedCamera = n;
                    scene->setActiveCamera(selectedCamera);
                    scene->needs_update = true;
                }
            }
}

void GUI::frustum_camera(Scene *scene)
{
    if(ImGui::Button("Draw Frustum")){
        scene->camera_frustum_mode = !scene->camera_frustum_mode;
        scene->needs_update = true;
    }
    ImGui::Text("%i", scene->camera_frustum_mode);
}

void GUI::toggle_view(Scene *scene)
{
    static bool projective =true;
    if(ImGui::Button("Projective View")){
        projective = !projective;
        scene->setCameraMode(scene->activeCamera,projective);
        scene->needs_update = true;
    }
    ImGui::SameLine();
    ImGui::Text("Projective Mode: %i", projective);
}

void GUI::perspective_camera(Camera *cam)
{
    static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
    static float slider_fovy = cam->getFOVY();
    static float slider_aspect_ratio = cam->getAspect();
    static float slider_near = cam->getNear();
    static float slider_far = cam->getFar();
    bool changed_perspective = false;
    slider_fovy = cam->getFOVY();
    slider_aspect_ratio = cam->getAspect();
    slider_near = cam->getNear();
    slider_far = cam->getFar();
    if(ImGui::SliderFloat("FOVY (35 -> 55)", &slider_fovy, 35.0f, 55.0f, "%.3f", flags)) changed_perspective = true;
    if(ImGui::SliderFloat("Aspect ratio (0.1 -> 10)", &slider_aspect_ratio, 0.1f, 10.0f, "%.3f", flags)) changed_perspective = true;
    if(ImGui::SliderFloat("Near (0.001 -> 1.0)", &slider_near, 0.001f, 1.0f, "%.3f", flags)) changed_perspective = true;
    if(ImGui::SliderFloat("Far (10 -> 1000)", &slider_far, 10.0f, 1000.0f, "%.3f", flags)) changed_perspective = true;

    if(changed_perspective){
        cam->updatePerspective(slider_fovy, slider_aspect_ratio, slider_near, slider_far);

    }
}

void GUI::projective_camera(Camera *cam)
{
    static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
    static float slider_near;
    static float slider_far;

    static float slider_bottom_orthogonal;
    static float slider_top_orthogonal;
    static float slider_left_orthogonal;
    static float slider_right_orthogonal;


    bool changed_orthogonal = false;
    slider_near = cam->getNear();
    slider_far = cam->getFar();
    vec4 lrbt = cam->getLeftRightBottomTop();
    slider_bottom_orthogonal = lrbt[2];
    slider_top_orthogonal = lrbt[3];
    slider_left_orthogonal = lrbt[0];
    slider_right_orthogonal = lrbt[1];
    static float slider_scale = 1;

     if(ImGui::SliderFloat("Scale", &slider_scale, 0.3f, 3.0f, "%.3f", flags)) changed_orthogonal = true;

    if(ImGui::SliderFloat("Bottom", &slider_bottom_orthogonal, -10.0f, 10.0f, "%.3f", flags)) changed_orthogonal = true;
    if(ImGui::SliderFloat("Top", &slider_top_orthogonal, -10.0f, 10.0f, "%.3f", flags)) changed_orthogonal = true;
    if(ImGui::SliderFloat("Left", &slider_left_orthogonal, -10.0f, 10.0f, "%.3f", flags)) changed_orthogonal = true;
    if(ImGui::SliderFloat("Right", &slider_right_orthogonal, -10.0f, 10.0f, "%.3f", flags)) changed_orthogonal = true;

    if(ImGui::SliderFloat("Near (0.001 -> 1.0)", &slider_near, 0.001f, 1.0f, "%.3f", flags)) changed_orthogonal = true;
    if(ImGui::SliderFloat("Far (10 -> 1000)", &slider_far, 10.0f, 1000.0f, "%.3f", flags)) changed_orthogonal = true;

    if(changed_orthogonal){
        // cam->updateOrthogonal(slider_left_orthogonal, slider_right_orthogonal,
        // 					  slider_bottom_orthogonal, slider_top_orthogonal,
        // 					  slider_near, slider_far);
        cam->updateFrustum(slider_scale*slider_left_orthogonal, slider_scale*slider_right_orthogonal,
                              slider_scale*slider_bottom_orthogonal, slider_scale*slider_top_orthogonal,
                              slider_near, slider_far);
    }


    ImGui::AlignTextToFramePadding();
    static float counter_x_near = cam->getNear();
    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    ImGui::Text("Near:");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##leftx_near", ImGuiDir_Left)) {
        counter_x_near-=0.01;
        slider_near-=0.01;
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##rightx_near", ImGuiDir_Right)) {
        counter_x_near+=0.01;
        slider_near+=0.01;
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%f", counter_x_near);


    ImGui::AlignTextToFramePadding();
    static float counter_x_bottom = slider_bottom_orthogonal;
    ImGui::Text("Bottom:");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##leftx_bottom", ImGuiDir_Left)) {
        counter_x_bottom-=0.01;
        slider_bottom_orthogonal-=0.01;
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##rightx_bottom", ImGuiDir_Right)) {
        counter_x_bottom+=0.01;
        slider_bottom_orthogonal+=0.01;
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%f", counter_x_bottom);


    if(ImGui::Button("Update Frustum")){
        cam->updateFrustum(slider_left_orthogonal, slider_right_orthogonal,
                              slider_bottom_orthogonal, slider_top_orthogonal,
                              slider_near, slider_far);
    }
}

void GUI::control_camera(Camera *cam)
{
    ImGui::AlignTextToFramePadding();
     static int counter_x,counter_y,counter_z = 0;
     float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    ImGui::Text("Tilt:");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##leftx", ImGuiDir_Left)) {
        counter_x--;
        cam->tilt(-0.1);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##rightx", ImGuiDir_Right)) {
        counter_x++;
        cam->tilt(0.1);
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", counter_x);

    ImGui::SameLine();
    ImGui::Text("Pan:");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##lefty", ImGuiDir_Left)) {
        counter_y--;
        cam->pan(-0.1);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##righty", ImGuiDir_Right)) {
        counter_y++;
        cam->pan(0.1);
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", counter_y);

    ImGui::SameLine();
    ImGui::Text("Cant:");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##leftz", ImGuiDir_Left)) {
        counter_z--;
        cam->cant(-0.1);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##rightz", ImGuiDir_Right)) {
        counter_z++;
        cam->cant(0.1);
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", counter_z);



    ImGui::AlignTextToFramePadding();
    static int counter_x_eye,counter_y_eye,counter_z_eye = 0;
    ImGui::Text("Truck:");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##leftx_eye", ImGuiDir_Left)) {
        counter_x_eye--;
        cam->truck(-1);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##rightx_eye", ImGuiDir_Right)) {
        counter_x_eye++;
        cam->truck(1);
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", counter_x_eye);

    ImGui::SameLine();
    ImGui::Text("Pedestal:");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##lefty_eye", ImGuiDir_Left)) {
        counter_y_eye--;
        cam->pedestal(-1);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##righty_eye", ImGuiDir_Right)) {
        counter_y_eye++;
        cam->pedestal(1);
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", counter_y_eye);

    ImGui::SameLine();
    ImGui::Text("Dolly:");
    ImGui::SameLine();
    ImGui::PushButtonRepeat(true);
    if (ImGui::ArrowButton("##leftz_eye", ImGuiDir_Left)) {
        counter_z_eye--;
        cam->dolly(-1);
    }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##rightz_eye", ImGuiDir_Right)) {
        counter_z_eye++;
        cam->dolly(1);
    }
    ImGui::PopButtonRepeat();
    ImGui::SameLine();
    ImGui::Text("%d", counter_z_eye);

ImGui::Text("Eye/At Location");
    vec3 ve= cam->getEye();
    static float vec_eye[4];
    for(int i=0; i<3;i++) {vec_eye[i] = ve[i];}
    vec_eye[3]=0.44f;
    ImGui::InputFloat3("Eye", vec_eye);
    vec3 va= cam->getAt();
    static float vec_at[4];
    for(int i=0; i<3;i++) {vec_at[i] = va[i];}
    vec_at[3]=0.44f;
    ImGui::InputFloat3("At", vec_at);

}



void GUI::LoadPrimitive(Scene* scene){
    ImGui::Text("Load Primitive:");
    const char* primitiveChoice[] = { "Cube", "Tetrahedron", "Sphere"};
    static int primitiveChoice_current = 0;
    ImGui::Combo("Type", &primitiveChoice_current, primitiveChoice, IM_ARRAYSIZE(primitiveChoice));
    static int numberPolygons = 3;
    if(primitiveChoice_current==2){
        ImGui::InputInt("input int", &numberPolygons);
    }
//    if(ImGui::Button("Add Primitive")){
//                            if(primitiveChoice_current==0)      scene->LoadPrimitive(Cube);
//                            else if(primitiveChoice_current==1) scene->LoadPrimitive(Tetrahedron);
//                            else if(primitiveChoice_current==2) scene->LoadPrimitive(Sphere, std::max(0,numberPolygons));
//                            LOG_ENGINE("Loaded Primitive.");
//                            //scene->initLastModel();
//                            scene->setActiveModel(scene->numberOfModels()-1);
//                            //scene->moveModel(scene->numberOfModels()-1, vec3(0,0,5)  );
//                        }

}

void GUI::loadOBJfile(Scene* scene){
    ImGui::Text("Load OBJ File:");
                        static bool texturedOption = false;
                        ImGui::Checkbox("Texture?", &texturedOption);
                        const bool browseButtonPressed = ImGui::Button("Add Model");
                        static ImGuiFs::Dialog dlg;
                        const char* chosenPath = dlg.chooseFileDialog(browseButtonPressed);
                        if (strlen(chosenPath)>0) {
                            LOG_ENGINE("Loaded obj model from path {}.",chosenPath);
                            scene->LoadOBJModel(chosenPath, texturedOption);
                            //scene->initLastModel(texturedOption);
                            scene->setActiveModel(scene->numberOfModels()-1);
                            //scene->moveModel(scene->numberOfModels()-1, vec3(0,0,5)  );
                        }

                        if (strlen(dlg.getChosenPath())>0) {
                        ImGui::Text("Chosen file: \"%s\"",dlg.getChosenPath());
                        }

                        static int selectedModel = -1;
                        for (int n = 0; n <  scene->numberOfModels(); n++)
                        {
                            char buf[32];
                            sprintf(buf, "Model %d", n);
                            if (ImGui::Selectable(buf, selectedModel == n)){
                                selectedModel = n;
                                scene->setActiveModel(selectedModel);
                                }
                        }
                        if(scene->numberOfModels())
                        {
                        ImGui::Text("Active Model: \"%i\"",scene->activeModel);
                        }
}

void GUI::focusCamera(Scene* scene){
    ImGui::Text("Focus Camera:");
    if(ImGui::Button("Focus Camera")){
  //      scene->lookAtModel(scene->activeCamera, scene->activeModel);
    }
    scene->needs_update = true;
}

void GUI::rotateModelLocalSpace(){
    ImGui::Text("Rotate in Local Space");
                        ImGui::AlignTextToFramePadding();

                        static ButtonLRarrow rollButton("Roll");
                        rollButton.left(eventmanager,
                                    Event(EventType::RotateModel, EventData(RotateDirection::ROLL, -5.0)));
                        rollButton.right(eventmanager,
                                    Event(EventType::RotateModel, EventData(RotateDirection::ROLL, 5.0)));


                        static ButtonLRarrow pitchButton("Pitch");
                        pitchButton.left(eventmanager,
                                    Event(EventType::RotateModel, EventData(RotateDirection::PITCH, -5.0)));
                        pitchButton.right(eventmanager,
                                    Event(EventType::RotateModel, EventData(RotateDirection::PITCH, 5.0)));


                        static ButtonLRarrow yawButton("Yaw");
                        yawButton.left(eventmanager,
                                    Event(EventType::RotateModel, EventData(RotateDirection::YAW, -5.0)));
                        yawButton.right(eventmanager,
                                    Event(EventType::RotateModel, EventData(RotateDirection::YAW, 5.0)));
}

//void GUI::translateModelWorldSpace(Scene* scene){
//    MeshModel* model = scene->getActiveModel();
//    static float stepsize = 1.0;
//    ImGui::Text("Translate in World Space");
//                        float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
//                        static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
//                        static float slider_step_size = 1.0f;
//                        ImGui::SliderFloat("Step size", &slider_step_size, 0.1f, 100.0f, "%.3f", flags);
//                        stepsize = slider_step_size;

//                        ImGui::AlignTextToFramePadding();
//                         static int counter_x_translate,counter_y_translate,counter_z_translate = 0;
//                        ImGui::Text("X:");
//                        ImGui::SameLine();
//                        ImGui::PushButtonRepeat(true);
//                        if (ImGui::ArrowButton("##leftx_model_translate", ImGuiDir_Left)) {
//                            counter_x_translate--;
//                            model->moveWorld(Translate(-vec3(stepsize,0,0)));
//                        }
//                        ImGui::SameLine(0.0f, spacing);
//                        if (ImGui::ArrowButton("##rightx_model_translate", ImGuiDir_Right)) {
//                            counter_x_translate++;
//                            model->moveWorld(Translate(vec3(stepsize,0,0)));
//                        }
//                        ImGui::PopButtonRepeat();
//                        ImGui::SameLine();
//                        ImGui::Text("%d", counter_x_translate);

//                        ImGui::SameLine();
//                        ImGui::Text("Y:");
//                        ImGui::SameLine();
//                        ImGui::PushButtonRepeat(true);
//                        if (ImGui::ArrowButton("##lefty_model_translate", ImGuiDir_Left)) {
//                            counter_y_translate--;
//                            model->moveWorld(Translate(vec3(0,-stepsize,0)));
//                        }
//                        ImGui::SameLine(0.0f, spacing);
//                        if (ImGui::ArrowButton("##righty_model_translate", ImGuiDir_Right)) {
//                            counter_y_translate++;
//                            model->moveWorld(Translate(vec3(0,stepsize,0)));
//                        }
//                        ImGui::PopButtonRepeat();
//                        ImGui::SameLine();
//                        ImGui::Text("%d", counter_y_translate);

//                        ImGui::SameLine();
//                        ImGui::Text("Z:");
//                        ImGui::SameLine();
//                        ImGui::PushButtonRepeat(true);
//                        if (ImGui::ArrowButton("##leftz_model_translate", ImGuiDir_Left)) {
//                            counter_z_translate--;
//                            model->moveWorld(Translate(vec3(0,0,-stepsize)));
//                        }
//                        ImGui::SameLine(0.0f, spacing);
//                        if (ImGui::ArrowButton("##rightz_model_translate", ImGuiDir_Right)) {
//                            counter_z_translate++;
//                            model->moveWorld(Translate(vec3(0,0,stepsize)));
//                        }
//                        ImGui::PopButtonRepeat();
//                        ImGui::SameLine();
//                        ImGui::Text("%d", counter_z_translate);
//}

//void GUI::translateModelLocalSpace(Scene* scene){
//                         ImGui::Text("Scale in Local Space");

//                                            MeshModel* model = scene->getActiveModel();

//                                            float spacing = ImGui::GetStyle().ItemInnerSpacing.x;

//                                            const float f = 1.1;

//                                            ImGui::Text("Uniform");
//                                            ImGui::SameLine();
//                                            static float counter_scale=1;
//                                            ImGui::PushButtonRepeat(true);
//                                            if (ImGui::ArrowButton("##left_model_scale", ImGuiDir_Left)) {
//                                                counter_scale*=1/f;
//                                                model->moveLocal(Scale(vec3(1/f)));
//                                            }
//                                            ImGui::SameLine(0.0f, spacing);
//                                            if (ImGui::ArrowButton("##right_model_scale", ImGuiDir_Right)) {
//                                                counter_scale*=f;
//                                                model->moveLocal(Scale(vec3(f)));
//                                            }
//                                            ImGui::PopButtonRepeat();
//                                            ImGui::SameLine();
//                                            ImGui::Text("%f", counter_scale);

//                                            ImGui::Text("Z");
//                                            ImGui::SameLine();
//                                            static float counter_scale_z=1;
//                                            ImGui::PushButtonRepeat(true);
//                                            if (ImGui::ArrowButton("##left_model_scale_z", ImGuiDir_Left)) {
//                                                counter_scale_z*=1/f;
//                                                model->moveLocal(Scale(vec3(1,1,1/f)));
//                                            }
//                                            ImGui::SameLine(0.0f, spacing);
//                                            if (ImGui::ArrowButton("##right_model_scale_z", ImGuiDir_Right)) {
//                                                counter_scale_z*=f;
//                                                model->moveLocal(Scale(vec3(1,1,f)));
//                                            }
//                                            ImGui::PopButtonRepeat();
//                                            ImGui::SameLine();
//                                            ImGui::Text("%f", counter_scale_z);
//}


//void GUI::drawTextureControl(Scene *scene)
//{
//    static bool rendered_texturedOption = false;
//    if(ImGui::Checkbox("Model Texture = Render to Texture", &rendered_texturedOption)){
//        if(rendered_texturedOption && !scene->models.empty()){
//            MeshModel* model = scene->models.back();
//            if(model->modelTexturedData){
//                model->tex_ = kipod::TextureManager::Get(0);
//                model->modelTexturedData->texture_ = *model->tex_;
//            }// Has a Texture
//        }//Yes and non-empty
//    }//Checkbox

//    static bool renderedToTextureScene = scene->pointsetToTexture_mode;
//    if(ImGui::Checkbox("Activate Render to Texture", &renderedToTextureScene)){
//            scene->pointsetToTexture_mode = !scene->pointsetToTexture_mode;
//            scene->_glrenderer->SwapPrograms();
//    }
//}

void GUI::drawBoundingBox(Scene *scene)
{
    ImGui::Text("Draw Bounding box");
                        if(ImGui::Button("Bounding Box on/off")){
                            scene->box_mode=!scene->box_mode;
                        }
                        ImGui::SameLine();
                        ImGui::Text("%i", scene->box_mode);
}

void GUI::drawNormals(Scene* scene){
    if(scene->numberOfModels()){
        MeshModel* model = scene->getActiveModel();
        float spacing = ImGui::GetStyle().ItemInnerSpacing.x;

        ImGui::Text("Draw Normals");
                            if(ImGui::Button("Normals on/off")){
                                scene->normals_mode=!scene->normals_mode;
                            }
                            ImGui::SameLine();
                            ImGui::Text("%i", scene->normals_mode);
//        ImGui::Text("Normal Length");
//                             static float normal_length=1;
//                            ImGui::PushButtonRepeat(true);
//                            if (ImGui::ArrowButton("##left_normal_length", ImGuiDir_Left)) {
//                                normal_length-=0.01;
//                                model->normal_length -=0.01;
//                            }
//                            ImGui::SameLine(0.0f, spacing);
//                            if (ImGui::ArrowButton("##right_normal_length", ImGuiDir_Right)) {
//                                normal_length+=0.01;
//                                model->normal_length +=0.01;
//                            }
//                            ImGui::PopButtonRepeat();
//                            ImGui::SameLine();
//                            ImGui::Text("%f", normal_length);
  }
}





//            if(ImGui::Button("Draw Line")){
//                line_active = !line_active;
//            }
//                if(line_active){
//                    if(!first_point)
//                        ImGui::Text("Choose first point:");
//                    else if(!second_point)
//                        ImGui::Text("Choose second point:");
//                    else if(first_point && second_point)
//                    {
//                        scene->drawLine(p,q);
//                        first_point=false;
//                        second_point=false;
//                        line_active = false;
//                    }
//                }

//              if(ImGui::TreeNode("Some Test Lines:"))
//                        {
//                        if(ImGui::Button("m=1")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 200; q[1] = 200;
//                            scene->drawLine(p,q);
//                        }
//                        if(ImGui::Button("m=2")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 200; q[1] = 400;
//                            scene->drawLine(p,q);
//                        }
//                        if(ImGui::Button("m=-1")){
//                            p[0] = 1; p[1] = 200;
//                            q[0] = 200; q[1] = 1;
//                            scene->drawLine(p,q);
//                        }
//                        if(ImGui::Button("m=-2")){
//                            p[0] = 1; p[1] = 400;
//                            q[0] = 200; q[1] = 1;
//                            scene->drawLine(p,q);
//                        }
//                        if(ImGui::Button("m=0")){
//                            p[0] = 1; p[1] = 200;
//                            q[0] = 100; q[1] = 200;
//                            scene->drawLine(p,q);
//                        }
//                        if(ImGui::Button("m = infty")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 1; q[1] = 200;
//                            scene->drawLine(p,q);
//                        }
//                        ImGui::TreePop();
//                    }
//              if(ImGui::TreeNode("Some Test Lines (DDA):"))
//                        {
//                        if(ImGui::Button("m=1")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 200; q[1] = 200;
//                            scene->drawLineDDA(p,q);
//                        }
//                        if(ImGui::Button("m=2")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 200; q[1] = 400;
//                            scene->drawLineDDA(p,q);
//                        }
//                        if(ImGui::Button("m=-1")){
//                            p[0] = 1; p[1] = 200;
//                            q[0] = 200; q[1] = 1;
//                            scene->drawLineDDA(p,q);
//                        }
//                        if(ImGui::Button("m=-2")){
//                            p[0] = 1; p[1] = 400;
//                            q[0] = 200; q[1] = 1;
//                            scene->drawLineDDA(p,q);
//                        }
//                        if(ImGui::Button("m=0")){
//                            p[0] = 1; p[1] = 200;
//                            q[0] = 100; q[1] = 200;
//                            scene->drawLineDDA(p,q);
//                        }
//                        if(ImGui::Button("m = infty")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 1; q[1] = 200;
//                            scene->drawLineDDA(p,q);
//                        }
//                        ImGui::TreePop();
//                    }
//              if(ImGui::TreeNode("Some Test Lines (Midpoint Incremental:"))
//                        {
//                        if(ImGui::Button("m=1")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 200; q[1] = 200;
//                            scene->drawLineMidpoint(p,q);
//                        }
//                        if(ImGui::Button("m=2")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 200; q[1] = 400;
//                            scene->drawLineMidpoint(p,q);
//                        }
//                        if(ImGui::Button("m=-1")){
//                            p[0] = 1; p[1] = 200;
//                            q[0] = 200; q[1] = 1;
//                            scene->drawLineMidpoint(p,q);
//                        }
//                        if(ImGui::Button("m=-2")){
//                            p[0] = 1; p[1] = 400;
//                            q[0] = 200; q[1] = 1;
//                            scene->drawLineMidpoint(p,q);
//                        }
//                        if(ImGui::Button("m=0")){
//                            p[0] = 1; p[1] = 200;
//                            q[0] = 100; q[1] = 200;
//                            scene->drawLineMidpoint(p,q);
//                        }
//                        if(ImGui::Button("m = infty")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 1; q[1] = 200;
//                            scene->drawLineMidpoint(p,q);
//                        }
//                        ImGui::TreePop();
//                    }
//              if(ImGui::TreeNode("Some Test Lines (Bresenheim:"))
//                        {
//                        if(ImGui::Button("m=1")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 200; q[1] = 200;
//                            scene->drawLineBresenheim(p,q);
//                        }
//                        if(ImGui::Button("m=1/2")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 200; q[1] = 100;
//                            scene->drawLineBresenheim(p,q);
//                        }
//                        if(ImGui::Button("m=2")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 200; q[1] = 400;
//                            scene->drawLineBresenheim(p,q);
//                        }
//                        if(ImGui::Button("m=-1/2")){
//                            p[0] = 1; p[1] = 200;
//                            q[0] = 200; q[1] = 100;
//                            scene->drawLineBresenheim(p,q);
//                        }
//                        if(ImGui::Button("m=-1")){
//                            p[0] = 1; p[1] = 200;
//                            q[0] = 200; q[1] = 1;
//                            scene->drawLineBresenheim(p,q);
//                        }
//                        if(ImGui::Button("m=-2")){
//                            p[0] = 1; p[1] = 400;
//                            q[0] = 200; q[1] = 1;
//                            scene->drawLineBresenheim(p,q);
//                        }
//                        if(ImGui::Button("m=0")){
//                            p[0] = 1; p[1] = 200;
//                            q[0] = 100; q[1] = 200;
//                            scene->drawLineBresenheim(p,q);
//                        }
//                        if(ImGui::Button("m = infty")){
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 1; q[1] = 200;
//                            scene->drawLineBresenheim(p,q);
//                        }
//                        ImGui::TreePop();
//                    }
//            if(ImGui::Button("Draw Coloured Test Triangle"))
//            {
//                scene->drawTestTriangle();
//            }
//            if(ImGui::Button("Draw Test Triangle"))
//            {
//                scene->drawTestTriangle2();
//            }
//            if(ImGui::TreeNode("Clip some Lines:"))
//                      {
//                        if(ImGui::Button("Clip a Line"))
//                        {
//                            int rect[4] = {100,400,100,400};
//                            p[0] = 1; p[1] = 1;
//                            q[0] = 500; q[1] = 500;

//                            if(CohenSutherlandClipping(p,q,rect))
//                            {
//                                scene->drawLine(p,q);
//                            }
//                        }
//                        if(ImGui::Button("Clip vertical Line"))
//                        {
//                            int rect[4] = {100,400,100,400};
//                            p[0] = 200; p[1] = 1;
//                            q[0] = 200; q[1] = 500;

//                            if(CohenSutherlandClipping(p,q,rect))
//                            {
//                                scene->drawLine(p,q);
//                            }
//                        }
//                        if(ImGui::Button("Clip horizontal Line"))
//                        {
//                            int rect[4] = {100,400,100,400};
//                            p[0] = 1; p[1] = 200;
//                            q[0] = 500; q[1] = 200;

//                            if(CohenSutherlandClipping(p,q,rect))
//                            {
//                                scene->drawLine(p,q);
//                            }
//                        }
//                        ImGui::TreePop();
//                    }

