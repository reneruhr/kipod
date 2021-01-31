#include "../include/pointset.h"


void PointSet::Init()
{
    LOG_ENGINE("Initialize PointSet");
    MakeSample();

    std::string name = "PointSet";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupPointSet(&sample_);
    AddLayout(name, layout);

    lattice_data_ =  std::make_shared<LatticeData>();
    lattice_data_->size = size(sample_);
}

void PointSet::Draw()
{
   RenderObject::Draw("PointSet");
}


void PointSet::BaseChange(GLRenderer *glrenderer, mat4 new_basis){
    basis_ = new_basis;
    sample_= {};
    MakeSample();
    glrenderer->UpdatePoints(this);
}
