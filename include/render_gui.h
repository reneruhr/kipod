#pragma once
#include "render_window.h"
#include "render_scene.h"
#include "engine_controls.h"

namespace kipod{
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

    static bool CollapsingHeader(const char*);

    static bool TreeNode(const char*);
    static void TreePop();

    static void CreateSceneWindow(kipod::RenderScene*);
};

}
