#include "../include/core.h"
#include "../include/scene.h"
#include "../include/window.h"
#include "../include/gui.h"
#include "../include/inputmanager.h"
#include "../include/eventmanager.h"

#include "../include/graphicsalgorithms.h"

#include "../include/quacry.h"
#include "../include/math/minkowski_embedding.h"

#include "../include/math/polygon.h"

#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
#include "../vendor/imgui/imguifilesystem.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))


Scene *scene;
GLRenderer *renderer;
SoftRenderer *softrenderer;
GUI *gui;
Window *window;
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

static bool first_point,second_point = false;

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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    glEnable(GL_DEPTH_TEST);
    scene->draw();

    gui->draw(scene, softrenderer, window);
}

void reshape( int width, int height )
{
//update the renderer's buffers
}

void mouse(int button, int state, int x, int y)
{

}

void motion(int x, int y)
{
	// calc difference in mouse movement
	int dx=x-last_x;
	int dy=y-last_y;
	// update last x,y
	last_x=x;
	last_y=y;
}

void processInput(GLFWwindow *window)
{

}

static double x,y;

void screenToPixel_x(double& x, int& p){
	p = (int)(x);
}

void screenToPixel_y(double& y, int& q){
    q = (int)(GLOBAL_SCR_HEIGHT-y);
}


int my_main( int argc, char **argv )
{

    window = new Window(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT, "NIR(renderer)");
    window->init();

    renderer = new GLRenderer(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
    softrenderer = new SoftRenderer(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);

    scene = new Scene(renderer, softrenderer, GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
    scene->init();

    eventmanager = new EventManager();
    eventmanager->addListener(scene);

    gui = new GUI(eventmanager);
    gui->init(window);


    inputmanager = new InputManager(eventmanager);
    inputmanager->init(window);

    mat4 amman_benker = MinkowskiEmbedding(2).Embedding();
//    QuaCry* quacry = new QuaCry(scene,
//                                amman_benker,
//                                {-10,10,-10,10,-5,5,-1,1},
//                                {-10,10,-10,10,-10,10,0,0},
//                                WindowType::Box);

    QuaCry* quacryOctagon = new QuaCry(scene,
                                amman_benker,
                                {-10,10,-10,10,-5,5,-5,5},
                                {-10,10,-10,10,-10,10,0,0},
                                WindowType::Octagon);
    quacryOctagon->window_vertices = Octagon(sqrt(2)).vertices_;

    //gui->AppendModule(quacry);
    gui->AppendModule(quacryOctagon);


    while (!window->windowShouldClose())
    {
        display();
        window->updateWindow();
        eventmanager->process();
    }


	delete scene;
	delete renderer;
    delete softrenderer;
    delete gui;
    delete window;
    delete inputmanager;

	return 0;
}


using namespace std;

int main( int argc, char **argv )
{
	
	Log::Init();
	LOG_INFO("Logger Started.");

	my_main(argc, argv );
	
	return 0;
}
