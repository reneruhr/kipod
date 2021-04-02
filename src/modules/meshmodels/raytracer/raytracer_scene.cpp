#include "raytracer_scene.h"
#include "../meshmodel_scene.h"
#include "../../../render/raytracer/raytracer_layout.h"

namespace kipod::MeshModels{

void RaytracerScene::Setup()
{

}

void RaytracerScene::Draw()
{
    raytracer_->ClearBuffer();
        for(const auto& model : scene_->render_objects_){
            raytracer_->SetUniforms(scene_->GetActiveCamera(), mat4( model->Transform() ));




        }
    scene_->framebuffer_->Bind();    
    glViewport(0, 0, scene_->width_, scene_->height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    raytracer_->DrawToOpenGL();
    kipod::RenderManager::Bind(0);

}



void RaytracerScene::CreatePrimitiveModelLayout(PrimMeshModel *model)
{
    // std::string name = "SoftLayout";
    // auto layout = new kipod::SoftRenderLayout();
    // layout->SetRaytracer(raytracer_.get());
    // layout->SetBuffer(model->vertices_vector,
    //           model->indices_vector,
    //           model->normals_vector,
    //           model->nindices_vector);
    // model->AddLayout(name, std::move(*layout));

}


RaytracerScene::RaytracerScene(MeshModelScene *scene) :
    MeshModelAPIScene(scene),
    raytracer_(std::make_unique<Raytracer>(scene->width_, scene->height_))
{

}


}
