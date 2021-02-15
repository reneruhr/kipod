#include "engine_gui.h"

namespace kipod{

void kipod::Gui::BeginWindow(const char * name)
{
    ImGui::Begin(name);
}

void kipod::Gui::EndWindow()
{
    ImGui::End();
}

void kipod::Gui::Checkbox(ModeToggle& toggle)
{
    if(ImGui::Checkbox(toggle.name_.c_str(), &toggle.state_))
        toggle.Apply();
}

void kipod::Gui::Transform(kipod::Transform& transform)
{

    float x = 0, y = 0, z = 0;
    float stepsize = 1.0f;
    bool pressed = false;

    ImGui::Text("x:");
    ImGui::SameLine();
    pressed =  ArrowButtons(&transform.x(), x , stepsize) || pressed;
    ImGui::SameLine();
    ImGui::Text("%f", transform.x());

    ImGui::Text("y:");
    ImGui::SameLine();
    pressed = ArrowButtons(&transform.y(), y , stepsize) || pressed;
    ImGui::SameLine();
    ImGui::Text("%f", transform.y());

    ImGui::Text("z:");
    ImGui::SameLine();
    pressed = ArrowButtons(&transform.z(), z , stepsize) || pressed;
    ImGui::SameLine();
    ImGui::Text("%f", transform.z());

    if(pressed) { LOG_ENGINE("Pressed Transform Arrow Button"); transform.Translate({x,y,z}); }
}

bool kipod::Gui::CollapsingHeader(const char* name)
{
    return ImGui::CollapsingHeader(name);
}

bool kipod::Gui::TreeNode(const char* name)
{
    return ImGui::TreeNode(name);
}

void kipod::Gui::TreePop()
{
    ImGui::TreePop();
}

bool Gui::ArrowButtons(float *f, float& value, float stepsize)
{
    bool pressed = false;
    ImGui::PushButtonRepeat(true);
    ImGui::PushID(f);
    if (ImGui::ArrowButton("left", ImGuiDir_Left)) {
        value-=stepsize;
        pressed = true;
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("right", ImGuiDir_Right)) {
        value+=stepsize;
        pressed = true;
    }
    ImGui::PopButtonRepeat();
    ImGui::PopID();
    return pressed;
}



void SetGuiColors();

kipod::Gui::~Gui()
{
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
}

void kipod::Gui::Init(std::shared_ptr<kipod::Window> window)
{
    Get();
    Gui::window_ = window;
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

    SetGuiColors();

}

void kipod::Gui::Begin()
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    ImGuiIO &io = ImGui::GetIO();
    static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    static bool dockspace = true;



    ImGuiViewport* viewport = ImGui::GetMainViewport();
//    ImGui::SetNextWindowPos(viewport->GetWorkPos());
//    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    //window_flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;


    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspace, window_flags);
    ImGui::PopStyleVar();


    ImGui::PopStyleVar(2);

    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("DockSpace_ID");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

}

void kipod::Gui::End()
{
        ImGui::Begin("ImGUI Demo");
        static bool show_demo_ = false;
        if(ImGui::Button("ShowDemoWindow"))    show_demo_ = !show_demo_;
        if(show_demo_) ImGui::ShowDemoWindow();
        ImGui::End();


        ImGui::End();


        ImGuiIO &io = ImGui::GetIO();

        io.DisplaySize = ImVec2((float)window_->Width(), (float)window_->Height());

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



void kipod::Gui::CreateSceneWindow(kipod::RenderScene* scene)
{
    ImGui::SetNextWindowSize(ImGui::GetContentRegionAvail(), ImGuiCond_FirstUseEver);
    ImGui::Begin("Module Viewport", (bool*)false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
    ImVec2 viewport_size = ImGui::GetContentRegionAvail();
    unsigned int x = static_cast<unsigned int>(viewport_size.x);
    unsigned int y = static_cast<unsigned int>(viewport_size.y);
    if(scene->width_ != x)   {
        LOG_ENGINE("Viewport Resized w={} h={}",x,y);
        scene->Resize(x, y);

    }
    unsigned int scene_texture = scene->SceneAsFramebuffer();
    ImGui::Image(reinterpret_cast<void*>(scene_texture), ImVec2(scene->width_,scene->height_), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
}





void SetGuiColors(){
    // FROM HAZEL:

    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
    //colors[ImGuiCol_WindowBg] = ImVec4{ 0.0f, 0.0f, 0.0f, 1.0f };

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


}
