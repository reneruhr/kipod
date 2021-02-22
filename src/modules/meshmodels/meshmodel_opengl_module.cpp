#include "meshmodel_opengl_module.h"
#include "meshmodel_opengl_scene.h"
#include "meshmodel_sidebar.h"
#include "meshmodel_console.h"


MeshModelModule::MeshModelModule(int width, int height)
{
    LOG_INFO("Meshmodel OpenGL Render Module constructed");
    scene_ = std::make_shared<MeshModelOpenGLScene>(MeshModelOpenGLScene(width, height));
    sidebar_ = std::make_unique<MeshmodelSidebar>(MeshmodelSidebar(scene_));
    console_ = std::make_unique<MeshModelConsole>(MeshModelConsole(scene_));
}
