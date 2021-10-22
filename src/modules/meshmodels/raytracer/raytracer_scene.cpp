#include "raytracer_scene.h"
#include "../meshmodel_scene.h"
#include "../../../render/raytracer/raytracer_layout.h"
//#include "../../../math/glmtosiegel.h"
namespace kipod::MeshModels{

glm::vec3 RaytracerScene::Screen(int i, int j)
{
    float x = i*1.0/scene_->width_;
    float y = j*1.0/scene_->height_;

    auto cam = scene_->GetActiveCamera();
    auto screen = cam->GetFrontScreen();

    auto hori = screen.right_bottom_ - screen.left_bottom_;
    auto vert = screen.left_top_ - screen.left_bottom_;

    return x*hori + y*vert + screen.left_bottom_;
}

void RaytracerScene::Setup()
{

}

void RaytracerScene::PrepareScreen()
{
    raytracer_->ClearBuffer();
    scene_->framebuffer_->Bind();    
    glViewport(0, 0, scene_->width_, scene_->height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}    

void RaytracerScene::Draw()
{
    auto cam = scene_->GetActiveCamera();
    for(int i = 0; i < scene_->width_; ++i){
        Vec3f color = {1.,1.,1.};
        raytracer_->DrawPoint(i,scene_->height_/2,&color);
    }
    for(int j = 0; j < scene_->height_; ++j){
        Vec3f color = {1.,1.,1.};
        raytracer_->DrawPoint(scene_->width_/2,j,&color);
    }
    for(const auto& model : scene_->render_objects_){
        PrimMeshModel* ray_object = dynamic_cast<PrimMeshModel*>(model.get());
        if(ray_object && ray_object->Type()==QuadricPrimitive){
            ray_object->GetRaytracingQuadric()->Transform(ray_object->Transform());
        }
    }
    for(int i = 0; i < scene_->width_; ++i){
        for(int j = 0; j < scene_->height_; ++j){

            auto origin = cam->Eye();
            auto direction = glm::normalize(Screen(i,j)-origin);
            Ray ray(origin, direction);

            /*
            LOG_DEBUG("Raytracing Screen: {} {} {}", Screen(i,j).x, Screen(i,j).y, Screen(i,j).z);
            LOG_DEBUG("Ray Origin {} {} {}", ray.Origin().x, ray.Origin().y, ray.Origin().z );
            LOG_DEBUG("Ray Direction {} {} {}", ray.Direction().x, ray.Direction().y, ray.Direction().z );
            */

            for(const auto& model : scene_->render_objects_){
                PrimMeshModel* ray_object = dynamic_cast<PrimMeshModel*>(model.get());
                if(ray_object){
                    if(ray_object->Type()==Sphere){
                        raytracer_->SetUniforms(cam, mat4( model->Transform() ));
                        RaytracingSphere sphere;
                        Intersections hit = Intersections(&ray, &sphere);
                        if(hit()){
                        /*  auto hit_point = hit.Point();
                            LOG_DEBUG("Hit at {} {} {}", hit_point.x, hit_point.y, hit_point.z ); */
                            Vec3f color = {1.,1.,1.};
                            raytracer_->DrawPoint(i,j,&color);
                        }
                    } else if(ray_object->Type()==QuadricPrimitive){
                        raytracer_->SetUniforms(cam, mat4( model->Transform() ));
                        RaytracingQuadric* quadric = ray_object->GetRaytracingQuadric();
                        Intersections hit = Intersections(&ray, quadric);
                        if(hit()){
                            Vec3f color = {1.,1.,1.};
                            raytracer_->DrawPoint(i,j,&color);
                        }
                    }
                    continue;
                }//primitive
                MeshModel* ray_model = dynamic_cast<MeshModel*>(model.get());
                if(ray_model){


                }//triangle
            }//model
        }//j
    }//i
    raytracer_->DrawToOpenGL();
}

void RaytracerScene::Resize(int w, int h)
{
    LOG_DEBUG("Creating new Raytracer");
    raytracer_= std::make_unique<Raytracer>(w, h);
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
