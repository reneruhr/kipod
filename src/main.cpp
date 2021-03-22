#include "kipod.h"

#include "modules/meshmodels/meshmodel_module.h"
#include "modules/shapes/shapes_module.h"
#include "modules/quasicrystals/quasicrystals_module.h"

#include "math/siegel2021/siegel2021.h"


extern unsigned int GLOBAL_SCR_WIDTH;
extern unsigned int GLOBAL_SCR_HEIGHT;


using namespace std;

int main( int argc, char **argv )
{

    int width = 1024, height = 768;
    kipod::Application kipod(width, height);
    kipod.Init();
    kipod.Add(kipod::MeshModels::MeshModelModule("Meshmodels", width,height));
    kipod.Add(kipod::Shapes::ShapesModule("2D Shapes", width,height));
    kipod.Add(kipod::QuasiCrystals::QuasiCrystalsModule("Quasi-Crystals", width,height));
    kipod.Run();
    kipod.ShutDown();

    return 0;
}
