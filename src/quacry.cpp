#include "../include/quacry.h"

#include "scene.h"
#include "../include/utils/mat.h"
#include "../include/pointset.h"


QuaCry::QuaCry(Scene *scene) : scene_(scene)
{
        mat4 basis(1.0);
        pointset_ = new PointSet(basis);
        pointset_->init(scene_->_glrenderer);
        scene_->pointsets.push_back(pointset_);

        window_box_ = new PrimMeshModel(Primitive::Cube);

        window_size_ =mat4(vec4{10.0,10.0,1.0,1.0});
        window_box_->moveLocal(window_size_);
        window_box_->setUniformMaterial();
        scene->models.push_back(window_box_);
        scene->models.back()->init(scene->_glrenderer);

        LOG("Inititalized QuaCry");
}
