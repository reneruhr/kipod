#include "kipod.h"

#include "modules/meshmodels/meshmodel_module.h"
#include "modules/shapes/shapes_module.h"
#include "modules/quasicrystals/quasicrystals_module.h"
#include "modules/image_processing/image_processing_module.h"

#include "siegel2021.h"


extern unsigned int GLOBAL_SCR_WIDTH;
extern unsigned int GLOBAL_SCR_HEIGHT;


using namespace std;

int main( int argc, char **argv )
{

//    int width = 1024, height = 768;
    int width = 512, height = 384;
    kipod::Application kipod(width, height);
    kipod.Init();
    kipod.Add(kipod::MeshModels::MeshModelModule("Meshmodels", width,height));
    kipod.Add(kipod::Shapes::ShapesModule("2D Shapes", width,height));
    kipod.Add(kipod::QuasiCrystals::QuasiCrystalsModule("Quasi-Crystals", width,height));
    kipod.Add(kipod::ImageProcessing::ImageProcessingModule("Image Processing", width,height));
    kipod.Run();
    kipod.ShutDown();

    return 0;
}
