#ifndef GUI_H
#define GUI_H


#include "render_window.h"
#include "scene.h"
#include "eventmanager.h"

#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
#include "../vendor/imgui/imguifilesystem.h"

#include "guimodule.h"

class GUI
{

public:
    GUI(EventManager* eventmanager) : eventmanager(eventmanager) {}
    ~GUI();
    void init(kipod::Window *window);
    void draw(Scene* scene, SoftRenderer* softrenderer, kipod::Window *window);

    vector<GUIModule*> gui_modules_;

    void AppendModule(GUIModule* module){ gui_modules_.push_back(module); }


    void draw_menus(Scene *scene, SoftRenderer *softrenderer, kipod::Window *window);
    void drawCameraControl(Scene *scene);
    void drawModelControl(Scene *scene);
    void drawNormalControl(Scene *scene);
    void drawBBOXControl(Scene *scene);
    void drawSoftRendererControl(Scene *scene, SoftRenderer *softrenderer, kipod::Window *window);
    void drawMaterialsControl(Scene *scene);
    void drawLightControl(Scene *scene);

    void drawPointSetControl(Scene *scene);
    void drawYotamBirthday(Scene *scene);
    void drawTextureControl(Scene *scene);
    void Begin(kipod::Window *window);
    void Draw(Scene* scene, SoftRenderer* softrenderer, kipod::Window* window);
    void End(kipod::Window* window);
private:
    EventManager* eventmanager;

    void add_and_list_cameras(Scene *scene);
    void frustum_camera(Scene *scene);
    void toggle_view(Scene *scene);
    void perspective_camera(Camera *cam);
    void projective_camera(Camera *cam);
    void control_camera(Camera *cam);
    void loadPrimitive(Scene *scene);
    void loadOBJfile(Scene *scene);
    void focusCamera(Scene *scene);
    void rotateModelLocalSpace();
    void translateModelWorldSpace(Scene *scene);
    void translateModelLocalSpace(Scene *scene);
    void drawCameras(Scene *scene);
    void drawBoundingBox(Scene *scene);
    void drawNormals(Scene *scene);
    void selectLineAlgorithm(SoftRenderer *softrenderer);
    void softRenderScene(Scene *scene, kipod::Window *window);
    void addMaterials(Scene *scene);
    void lightControl(Scene *scene);
    void showMaterials(Scene *scene);
};

#endif // GUI_H
