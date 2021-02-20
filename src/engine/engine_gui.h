#pragma once
#include "../render/render_window.h"
#include "../render/render_scene.h"
#include "engine_controls.h"
#include "engine_transform.h"

namespace kipod{

void HelpMarker(const char* desc);

class Gui
{
    Gui(){}
    inline static std::shared_ptr<kipod::Window> window_;

public:
    Gui(Gui&) = delete;
    void operator=(Gui&)  = delete;
    ~Gui();

    static Gui& Get(){  static Gui Gui;   return Gui;  }
    static void Init(std::shared_ptr<kipod::Window> window_);
    static void Begin();
    static void End();

    static void BeginWindow(const char*);
    static void EndWindow();

    static void Checkbox(ModeToggle&);
    static void RadioButtons(MultipleModeToggle&, void* ptr);
    static void Transform(Transform&);
    static void Scale(kipod::Transform&);
    static void Color(RenderMaterial&);

    static bool CollapsingHeader(const char*);

    static bool TreeNode(const char*);
    static void TreePop();

    static bool ArrowButtons(float* id, float& value, float stepsize);

    static void CreateSceneWindow(kipod::RenderScene*);
};

}
