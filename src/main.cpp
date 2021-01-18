#define STB_IMAGE_IMPLEMENTATION

#include "../include/core.h"
#include "../include/scene.h"
#include "../include/window.h"
#include "../include/gui.h"
#include "../include/inputmanager.h"
#include "../include/eventmanager.h"

#include "../include/render_manager.h"
#include "../include/texture.h"

#include "../include/graphicsalgorithms.h"

#include "../include/quacry.h"
#include "../include/math/minkowski_embedding.h"

#include "../include/math/polygon.h"



#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
#include "../vendor/imgui/imguifilesystem.h"

#include <chrono>
#include <thread>


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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
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

    window = new Window(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT, "קיפוד(renderer)");
    window->init();

//    RenderManager::Init();
    auto gl_frame_buffer = RenderManager::addFrameBuffer();
//    TextureManager::Init();


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

    mat4 square_root = transpose(MinkowskiEmbedding(2).Embedding());




//    QuaCry* quacry = new QuaCry(scene,
//                                amman_benker,
//                                {-10,10,-10,10,-5,5,-1,1},
//                                {-10,10,-10,10,-10,10,0,0},
//                                WindowType::Box);
//    gui->AppendModule(quacry);

    QuaCry* quacryOctagon = new QuaCry(scene,
                                square_root,
                                {-10,10,-10,10,-5,5,-5,5},
                                {-20,20,-20,20,-10,10,-10,10},
                                WindowType::Octagon,
                                Shape( Octagon(sqrt(2)) ) );
    quacryOctagon->ScaleShape(1.0f/20.0f);
    quacryOctagon->Move({0.8f,0.8f});

   gui->AppendModule(quacryOctagon);

   RenderManager::addFrameBuffer();
   Texture* quasi_texture = new Texture(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
   quasi_texture->RenderToTexture(*RenderManager::Get(1));
   TextureManager::Add(quasi_texture);



    double then = glfwGetTime();
    double now;
    double limit = 1/30.0; // seconds per frame
    double gap;

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    while (!window->windowShouldClose())
    {
//        double currentTime = glfwGetTime();
//        nbFrames++;
//        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
//            // printf and reset timer
//            printf("%f ms/frame\n", 1000.0/double(nbFrames));
//            nbFrames = 0;
//            lastTime += 1.0;
//        }

        now = glfwGetTime();
        gap = now-then;
        if(gap < limit){
            std::this_thread::sleep_for( std::chrono::milliseconds( (int)(1000*(limit-gap)) ) );
        }
        then = now;
        gui->Begin(scene, softrenderer, window);
        display();
        //gui->draw(scene, softrenderer, window);
        gui->End(window);
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
