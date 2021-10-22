#include "softrenderer_scene.h"
#include "../meshmodel_scene.h"
#include "../../../render/softrenderer/softrenderer_layout.h"

namespace kipod::MeshModels{

void SoftRendererScene::Setup()
{

}

void SoftRendererScene::PrepareScreen()
{
    softrenderer_->ClearBuffer();
    scene_->framebuffer_->Bind();    
    glViewport(0, 0, scene_->width_, scene_->height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}    

void SoftRendererScene::Draw()
{
        for(const auto& model : scene_->render_objects_){
            softrenderer_->SetUniforms(scene_->GetActiveCamera(), mat4( model->Transform() ));
            if(scene_->Toggle("Colors") || scene_->Toggle("Emissive"))
                softrenderer_->DrawColoredTriangles(model.get(), scene_->lights_, scene_->Toggle("Emissive") );
            else
                softrenderer_->DrawTriangles(model.get(), scene_->Toggle("Wireframe"), scene_->Toggle("Normals") );

//            if(box_mode){
//                 mat4 m = mat4( &model->TansformBoundingBox()[0][0] ); //to be changed
//                 softrenderer_->SetObjectMatrices(m, mat3(1.0));
//                 boundingBox.draw(softrenderer_, true,false);
//            }
        }
    softrenderer_->DrawToOpenGL();
}


void SoftRendererScene::DrawBoundingBox(MeshModel *model, RenderCamera *camera)
{

}
void SoftRendererScene::DrawGrid(RenderCamera *camera)
{

}
void SoftRendererScene::DrawCoordinateAxis(RenderCamera *camera)
{

}

void SoftRendererScene::CreateMeshModelLayout(MeshModel *model)
{
    std::string name = "SoftLayout";
    auto layout = new kipod::SoftRenderLayout();
    layout->SetSoftRenderer(softrenderer_.get());
    layout->SetBuffer(model->vertices_vector,
              model->indices_vector,
              model->normals_vector,
              model->nindices_vector);
    model->AddLayout(name, std::move(*layout));

}
void SoftRendererScene::CreatePrimitiveModelLayout(PrimMeshModel *model)
{
    std::string name = "SoftLayout";
    auto layout = new kipod::SoftRenderLayout();
    layout->SetSoftRenderer(softrenderer_.get());
    layout->SetBuffer(model->vertices_vector,
              model->indices_vector,
              model->normals_vector,
              model->nindices_vector);
    model->AddLayout(name, std::move(*layout));

}
void SoftRendererScene::CreateCoordinateAxisLayout(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &colors)
{

}
void SoftRendererScene::CreateBoundingBoxLayout()
{

}
void SoftRendererScene::CreateGridLayout(std::vector<glm::vec3> &vertices)
{

}

SoftRendererScene::SoftRendererScene(MeshModelScene *scene) :
    MeshModelAPIScene(scene),
    softrenderer_(std::make_unique<SoftRenderer>(scene->width_, scene->height_))
{

}


}
