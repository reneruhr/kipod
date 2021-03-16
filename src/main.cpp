#include "kipod.h"

#include "modules/meshmodels/meshmodel_opengl_module.h"
#include "modules/shapes/shapes_module.h"
#include "modules/quasicrystals/quasicrystals_module.h"

#include "math/siegel2021/siegel2021.h"
#include "math/siegel2021/MatrixTest.h"

//#include "scene.h"
//#include "gui.h"
//#include "inputmanager.h"
//#include "eventmanager.h"

//#include "graphicsalgorithms.h"

//#include "quacry.h"
//#include "math/minkowski_embedding.h"


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))


extern unsigned int GLOBAL_SCR_WIDTH;
extern unsigned int GLOBAL_SCR_HEIGHT;
//----------------------------------------------------------------------------
// Callbacks

//void display(Scene* scene)
//{
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glPointSize(4);
//    glEnable(GL_PROGRAM_POINT_SIZE);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    if(scene->cg_active)
//        {
//            glViewport(GLOBAL_SCR_WIDTH, 0, GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
//            if(scene->needs_update){
//            scene->needs_update = false;
//            scene->clearBuffer();
//            scene->drawSoft();
//            }
//            scene->swapBuffers();
//		}

//    glViewport(0, 0, GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
//    scene->draw();
//}



//int my_main( int argc, char **argv )
//{
//    Scene *scene;
//    GLRenderer *renderer;
//    SoftRenderer *softrenderer;
//    GUI *gui;
//    std::shared_ptr<kipod::Window> window;
//    EventManager *eventmanager;

//    Log::Init();
//    LOG_ENGINE("Logger Started.");
//    window = std::shared_ptr<kipod::Window>(new kipod::Window(GLOBAL_SCR_WIDTH*1.1, GLOBAL_SCR_HEIGHT, "קיפוד(renderer)"));
//    window->init();


//    renderer = new GLRenderer(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
//    softrenderer = new SoftRenderer(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);

//    scene = new Scene(renderer, softrenderer, GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
//    scene->Setup();

//    eventmanager = new EventManager();
//    eventmanager->addListener(scene);

//    gui = new GUI(eventmanager);
//    //gui->init(window.get());






//   //????? Breaks Imgui Viewport
////   auto quasi_framebuffer = kipod::RenderManager::addFrameBuffer();
////   kipod::Texture* quasi_texture = new kipod::Texture(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
////   quasi_texture->RenderToTexture(quasi_framebuffer->opengl_id_);
////   kipod::TextureManager::Add(quasi_texture);



//    kipod::Clock clock;

//    kipod::Gui::Init(window);


//    while (!window->windowShouldClose())
//    {
////        kipod::Gui::Begin();
////        //kipod::Menu();

////        display(scene);


////        gui->Draw(scene, softrenderer, window.get());
////        scene->DrawGui();
////        kipod::Gui::CreateSceneWindow(scene);
////        kipod::Gui::End();

////        window->updateWindow();
////        eventmanager->process();
////        kipod::Events::Process();
////        clock.Synchronize();
//    }

//	delete scene;
//	delete renderer;
//    delete softrenderer;
//    delete gui;


//	return 0;
//}


using namespace std;

int main( int argc, char **argv )
{
    bool appmode = true;

    siegel2021::RunMatrixTest();

    if(appmode){
        int width = 1024, height = 768;
        kipod::Application kipod(width, height);
        kipod.Init();
        kipod.Add("OpenGL Meshmodels", kipod::MeshModels::MeshModelModule(width,height));
        kipod.Add("Shapes 2d", kipod::Shapes::ShapesModule(width,height));
        kipod.Add("Quasi-Crystals", kipod::QuasiCrystals::QuasiCrystalsModule(width,height));
        kipod.Run();
        kipod.ShutDown();
    }else    ;//my_main(argc, argv );
	
    return 0;
}
