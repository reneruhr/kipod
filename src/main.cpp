#define STB_IMAGE_IMPLEMENTATION

#include "kipod.h"

#include "scene.h"
#include "gui.h"
#include "inputmanager.h"
#include "eventmanager.h"

#include "graphicsalgorithms.h"

#include "quacry.h"
#include "math/minkowski_embedding.h"

#include "math/polygon.h"

#include "engine/engine_events.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

Scene *scene;
GLRenderer *renderer;
SoftRenderer *softrenderer;
GUI *gui;
kipod::Window *window;
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

    //scene->Draw();

}


void screenToPixel_x(double& x, int& p){
	p = (int)(x);
}

void screenToPixel_y(double& y, int& q){
    q = (int)(GLOBAL_SCR_HEIGHT-y);
}


int my_main( int argc, char **argv )
{
    window = new kipod::Window(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT, "קיפוד(renderer)");
    window->init();


    renderer = new GLRenderer(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
    softrenderer = new SoftRenderer(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);

    scene = new Scene(renderer, softrenderer, GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
    scene->Setup();

    eventmanager = new EventManager();
    eventmanager->addListener(scene);

    gui = new GUI(eventmanager);
    gui->init(window);


//    inputmanager = new InputManager(eventmanager);
//    inputmanager->init(window);

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


   //????? Breaks Imgui Viewport
//   auto quasi_framebuffer = kipod::RenderManager::addFrameBuffer();
//   Texture* quasi_texture = new kipod::Texture(GLOBAL_SCR_WIDTH, GLOBAL_SCR_HEIGHT);
//   quasi_texture->RenderToTexture(quasi_framebuffer->opengl_id_);
//   kipod::TextureManager::Add(quasi_texture);



    double then = glfwGetTime();
    double now;
    double limit = 1/30.0; // seconds per frame
    double gap;

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


        display();
        //gui->draw(scene, softrenderer, window);
        gui->Begin(window);
        gui->Draw(scene, softrenderer, window);
        scene->DrawGui();
        gui->End(window);

        window->updateWindow();
        eventmanager->process();
        kipod::Events::Process();


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
