#include "softrenderer_scene.h"

void kipod::SoftRenderer::SoftRendererScene::Setup()
{
        name_ = "SoftRenderer Scene";

        Signup();

        LOG_INFO("SoftRenderer Scene Initialized.");
        LOG_CONSOLE("SoftRenderer Scene Initialized.");
}

void kipod::SoftRenderer::SoftRendererScene::Draw()
{
    framebuffer_->Bind();
    glViewport(0, 0, width_, height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    mat4 p = cameras[activeCamera]->getProjection(camerasMode[activeCamera]);
    //    mat4 v = cameras[activeCamera]->getcTransform();
    //    _softrenderer->SetCameraTransform(v);
    //    _softrenderer->SetProjection(p);
    //    for(auto model : models){
    //        mat4 m = model->getmTransform();
    //        mat3 n = mat3(vec3(&m[0][0]),vec3(&m[1][0]),vec3(&m[2][0]));
    //        n = transpose(Inverse(n));
    //        _softrenderer->SetObjectMatrices(m,n);

    //        if(color_mode || emissive_mode)
    //            model->drawWithLight(_softrenderer,lights, color_mode, emissive_mode);
    //        else
    //            model->draw(_softrenderer, wireframemode, clipping_mode, normals_mode);

    //        if(box_mode){
    //             mat4 m = mat4( &model->TansformBoundingBox()[0][0] ); //to be changed
    //             _softrenderer->SetObjectMatrices(m, mat3(1.0));
    //             boundingBox.draw(_softrenderer, true,false);
    //        }
    //    }

    kipod::RenderManager::Bind(0);

}



void kipod::SoftRenderer::SoftRendererScene::ActiveMeshModel(MeshModel* active)
{
    active_meshmodel_= active;
}

kipod::SoftRenderer::MeshModel *kipod::SoftRenderer::SoftRendererScene::ActiveMeshModel()
{
    return active_meshmodel_;
}


bool kipod::SoftRenderer::SoftRendererScene::HasMeshModel()
{
    return active_meshmodel_!=nullptr;
}

void kipod::SoftRenderer::SoftRendererScene::ProcessKeys(kipod::KeyPressedEvent &event)
{
    float stepsize = 1.0f;
    auto key = event.GetKeyCode();

    if(HasMeshModel()){
        if(key == Key::Left){
                    ActiveMeshModel()->world_->Translate({-stepsize,0,0});
                }
        else if(key == Key::Right){
                    ActiveMeshModel()->world_->Translate({+stepsize,0,0});
                }
        else if(key == Key::Up){
                    ActiveMeshModel()->world_->Translate({0,stepsize,0});
                }
        else if(key == Key::Down){
                    ActiveMeshModel()->world_->Translate({0,-stepsize,0});
                }
        else if(key == Key::PageUp){
                    ActiveMeshModel()->world_->Translate({0,0, stepsize});
                }
        else if(key == Key::PageDown){
                    ActiveMeshModel()->world_->Translate({0,0,-stepsize});
                }
    }
}

void kipod::SoftRenderer::SoftRendererScene::SetupLayout(MeshModel* meshmodel)
{
    //meshmodel->Init();
}

void kipod::SoftRenderer::SoftRendererScene::SetupShader()
{
    shader_= std::make_shared<kipod::Shader>("softrenderer.vert.glsl",   "softrenderer.frag.glsl");
    shader_->AttachUniform<glm::mat4>("transform");
}


kipod::SoftRenderer::SoftRendererScene::SoftRendererScene(int width, int height):
    RenderScene(width, height){}

void kipod::SoftRenderer::SoftRendererScene::Signup()
{
    kipod::Events::Signup(this, kipod::EventCategoryKeyboard); LOG_CATEGORY_NAME(kipod::EventCategoryKeyboard);
}

void kipod::SoftRenderer::SoftRendererScene::Receive(std::shared_ptr<kipod::Event> event)
{
    Process<kipod::KeyPressedEvent>(event, BIND_EVENT_FN(SoftRendererScene::ProcessKeys));
}
