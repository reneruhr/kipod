#include "kipod.h"

#include "modules/meshmodels/meshmodel_opengl_module.h"


#include "scene.h"
#include "gui.h"
#include "inputmanager.h"
#include "eventmanager.h"

#include "graphicsalgorithms.h"

#include "quacry.h"
#include "math/minkowski_embedding.h"

#include "math/polygon.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

Scene *scene;
GLRenderer *renderer;
SoftRenderer *softrenderer;
GUI *gui;
std::shared_ptr<kipod::Window> window;
InputManager *inputmanager;
EventManager *eventmanager;


int last_x,last_y;
bool lb_down,rb_down,mb_down;

bool scene_active = false;
bool demo_active = false;
bool line_active = false;



vec3 zero = vec3(0.0f);
vec3 translateModel = zero;
vec3 translateEye = zero;

float translation_stepsize = 1.0;


int p[2];
int q[2];


extern unsigned int GLOBAL_SCR_WIDTH;
extern unsigned int GLOBAL_SCR_HEIGHT;
//----------------------------------------------------------------------------
// Callbacks

void display( void );
void reshape( int width, int height );
void keyboard( unsigned char key, int x, int y );
void mouse(int button, int state, int x, int y);
void fileMenu(int id);
void mainMenu(int id);

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(4);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(scene->cg_active)
        {
            glViewport(GLOBAL_SCR_WIDTH, 0, GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
            if(scene->needs_update){
            scene->needs_update = false;
            scene->clearBuffer();
            scene->drawSoft();
            }
            scene->swapBuffers();
		}

    glViewport(0, 0, GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
    scene->draw();
}



int my_main( int argc, char **argv )
{
    Log::Init();
    LOG_ENGINE("Logger Started.");
    window = std::shared_ptr<kipod::Window>(new kipod::Window(GLOBAL_SCR_WIDTH*1.1, GLOBAL_SCR_HEIGHT, "קיפוד(renderer)"));
    window->init();


    renderer = new GLRenderer(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
    softrenderer = new SoftRenderer(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);

    scene = new Scene(renderer, softrenderer, GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
    scene->Setup();

    eventmanager = new EventManager();
    eventmanager->addListener(scene);

    gui = new GUI(eventmanager);
    //gui->init(window.get());


    mat4 square_root = transpose(MinkowskiEmbedding(2).Embedding());

    QuaCry* quacryOctagon = new QuaCry(scene,
                                square_root,
                                {-10,10,-10,10,-5,5,-5,5},
                                {-20,20,-20,20,-10,10,-10,10},
                                WindowType::Octagon,
                                Shape( Octagon(sqrt(2)) ) );
    quacryOctagon->ScaleShape(1.0f/20.0f);
    quacryOctagon->Move({0.8f,0.8f});
   gui->AppendModule(quacryOctagon);


   //????? Breaks Imgui Viewport
//   auto quasi_framebuffer = kipod::RenderManager::addFrameBuffer();
//   Texture* quasi_texture = new kipod::Texture(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
//   quasi_texture->RenderToTexture(quasi_framebuffer->opengl_id_);
//   kipod::TextureManager::Add(quasi_texture);

    kipod::Clock clock;

    kipod::Gui::Init(window);


    while (!window->windowShouldClose())
    {
        kipod::Gui::Begin();
        kipod::Menu();

        display();


        gui->Draw(scene, softrenderer, window.get());
        scene->DrawGui();
        kipod::Gui::CreateSceneWindow(scene);
        kipod::Gui::End();

        window->updateWindow();
        eventmanager->process();
        kipod::Events::Process();
        clock.Synchronize();
    }

	delete scene;
	delete renderer;
    delete softrenderer;
    delete gui;
    delete inputmanager;

    glfwTerminate();
	return 0;
}


using namespace std;

int main( int argc, char **argv )
{
    bool appmode = false;

    if(appmode){
        int width = 1024, height = 768;
        kipod::Application kipod(width, height);
        kipod.Init();
        kipod.Add("OpenGL Meshmodels", MeshModelModule(width,height));
        kipod.Run();
    }else    my_main(argc, argv );
	
    return 0;
}
