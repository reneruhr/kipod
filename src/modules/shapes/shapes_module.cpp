#include "shapes_module.h"
#include "shapes_sidebar.h"
#include "shapes_scene.h"
namespace kipod::Shapes{

ShapesModule::ShapesModule(int width, int height)
{
    LOG_INFO("Meshmodel OpenGL Render Module constructed");
    scene_ = std::make_shared<ShapesScene>(ShapesScene(width, height));
    sidebar_ = std::make_unique<ShapesSidebar>(ShapesSidebar(scene_));
    console_ = nullptr;
}
}
