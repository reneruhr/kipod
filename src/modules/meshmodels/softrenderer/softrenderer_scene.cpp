#include "softrenderer_scene.h"
#include "../meshmodel_scene.h"

namespace kipod::MeshModels{

void SoftRendererScene::Setup()
{

}

void SoftRendererScene::Draw()
{
    scene_->framebuffer_->Bind();
    glViewport(0, 0, scene_->width_, scene_->height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        mat4 p = scene_->GetActiveCamera()->getProjection(camerasMode[activeCamera]);
//        mat4 v = scene_->GetActiveCamera()->getcTransform();
//        _softrenderer->SetCameraTransform(v);
//        _softrenderer->SetProjection(p);
//        for(auto model : models){
//            mat4 m = model->getmTransform();
//            mat3 n = mat3(vec3(&m[0][0]),vec3(&m[1][0]),vec3(&m[2][0]));
//            n = transpose(Inverse(n));
//            _softrenderer->SetObjectMatrices(m,n);

//            if(color_mode || emissive_mode)
//                model->drawWithLight(_softrenderer,lights, color_mode, emissive_mode);
//            else
//                model->draw(_softrenderer, wireframemode, clipping_mode, normals_mode);

//            if(box_mode){
//                 mat4 m = mat4( &model->TansformBoundingBox()[0][0] ); //to be changed
//                 _softrenderer->SetObjectMatrices(m, mat3(1.0));
//                 boundingBox.draw(_softrenderer, true,false);
//            }
//        }

    kipod::RenderManager::Bind(0);

}


void SoftRendererScene::SetupShader()
{
    shader_= std::make_shared<kipod::Shader>("softrenderer.vert.glsl",   "softrenderer.frag.glsl");
    shader_->AttachUniform<glm::mat4>("transform");
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

}
void SoftRendererScene::CreatePrimitiveModelLayout(PrimMeshModel *model)
{

}
void SoftRendererScene::CreateCoordinateAxisLayout(std::vector<vec3> &vertices, std::vector<vec3> &colors)
{

}
void SoftRendererScene::CreateBoundingBoxLayout()
{

}
void SoftRendererScene::CreateGridLayout(std::vector<vec3> &vertices)
{

}


}
