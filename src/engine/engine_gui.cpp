#include "engine_gui.h"
#include "../utils/imgui_utils.h"

namespace kipod{

void HoverInfo(const char* name, const char* desc)
{
    ImGui::TextDisabled("%s", name);
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void HelpMarker(const char* desc)
{
    HoverInfo("(?)", desc);
}



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

void Gui::RadioButtons(MultipleModeToggle& toggle, void* ptr)
{
    int size = toggle.toggle_names_.size();
    for(int i = 0; i<size; ++i){
        if(ImGui::RadioButton(toggle.toggle_names_[i].c_str(), &toggle.state_, i))
            toggle.Apply(ptr);
    }
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
    ImGui::Text("%.2f", transform.x());

    ImGui::Text("y:");
    ImGui::SameLine();
    pressed = ArrowButtons(&transform.y(), y , stepsize) || pressed;
    ImGui::SameLine();
    ImGui::Text("%.2f", transform.y());

    ImGui::Text("z:");
    ImGui::SameLine();
    pressed = ArrowButtons(&transform.z(), z , stepsize) || pressed;
    ImGui::SameLine();
    ImGui::Text("%.2f", transform.z());


    if(pressed) { LOG_ENGINE("Pressed Transform Arrow Button"); transform.Translate({x,y,z}); }
}

void Gui::Scale(kipod::Transform& transform)
{
    float stepsize = 0.05;;
    float s = 1.0;
    ImGui::Text("Scale:");
    ImGui::SameLine();
    if(ArrowButtons(&s, s, stepsize))
        transform.Scale({s,s,s});
}

void Gui::Color(RenderMaterial &material)
{
        ImGui::ColorEdit4("Ambient##1", (float*)&material.ambient_);
        ImGui::ColorEdit4("Diffuse##2", (float*)&material.diffuse_);
        ImGui::ColorEdit4("Specular##3", (float*)&material.specular_);
        ImGui::ColorEdit4("Emission##4", (float*)&material.emission_);
        ImGui::SliderFloat("Shininess", &material.shininess_, 0.0f, 500.0f, "%.1f");
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
    io.Fonts->AddFontFromFileTTF("resources/fonts/Roboto-Medium.ttf", 14.0f);
    io.Fonts->Build();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    io.WantCaptureKeyboard=1;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->_window, true);
    ImGui_ImplOpenGL3_Init(window->glsl_version.c_str());

    //SetGuiColors();
    //ImGui::SetupImGuiStyle(false, 0.9);
    ImGui::Setstyle();

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
    ImGui::SetNextWindowSize(ImVec2(scene->width_, scene->height_),ImGuiCond_FirstUseEver);
//    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Module Viewport", (bool*)false,  ImGuiWindowFlags_NoBackground); //ImGuiWindowFlags_NoScrollbar


    ImGuiIO &io = ImGui::GetIO();  (void)io;
    io.WantCaptureKeyboard=1;

    ImVec2 viewport_size = ImGui::GetContentRegionAvail();
    unsigned int x = static_cast<unsigned int>(viewport_size.x);
    unsigned int y = static_cast<unsigned int>(viewport_size.y);
    static unsigned int old_x = x; static unsigned int ancient_x = x; static bool old_or_ancient = false;

    if( x!= old_x && x!= ancient_x ) // Fixes some resizing bug from Imgui
        if(scene->width_ != x)   {
            LOG_ENGINE("Viewport Resized w={} h={}",x,y);
            scene->Resize(x, y);
            { // Fixes some resizing bug from Imgui
                if(old_or_ancient) old_x = x;
                else ancient_x = x;
                old_or_ancient=!old_or_ancient;
            }
        }
    unsigned int scene_texture = scene->SceneAsFramebuffer();
    ImGui::Image(reinterpret_cast<void*>(scene_texture), ImVec2(scene->width_ ,scene->height_), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
    ImGui::PopStyleVar();
}








}
