#include "meshmodel_opengl_scene.h"
#include "../meshmodel_scene.h"
namespace kipod::MeshModels{



/////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void OpenGLScene::Setup()
{
    SetupShaders();
    CreateBoundingBoxLayout();
    CreateBoundingSphereLayout();
}

void OpenGLScene::CreateBoundingBoxLayout(){
    std::string name = "Colored Triangles";
    auto layout = new kipod::GLRenderLayout;
    layout->sha_ = shaders_["Colored Triangles"];
    scene_->bounding_box_.AddLayout(name,std::move(*layout));

    std::string name2 = "Basic";
    auto layout2 = new kipod::GLRenderLayout;
    layout2->sha_ = shaders_["Basic"];
    scene_->bounding_box_.AddLayout(name2,std::move(*layout2));

    scene_->bounding_box_.Init(false,false,true);
}

void OpenGLScene::CreateBoundingSphereLayout(){
    std::string name = "Basic";
    auto layout = new kipod::GLRenderLayout;
    layout->sha_ = shaders_["Basic"];
    scene_->bounding_sphere_.AddLayout(name,std::move(*layout));
    scene_->bounding_sphere_.Init(false,false,true);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//DRAWING                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void OpenGLScene::Draw()
{
    scene_->framebuffer_->Bind();
    glViewport(0, 0, scene_->width_, scene_->height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    DrawCoordinateAxis(scene_->GetActiveCamera());
    DrawGrid(scene_->GetActiveCamera());
    glDisable(GL_DEPTH_TEST);

    for(const auto& model : scene_->render_objects_){
        if(scene_->Toggle("Wireframe")) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glEnable(GL_DEPTH_TEST);

        if( scene_->Toggle("Textures") && model->HasLayout("Textured Triangles") ){
           shaders_["Textured Triangles"]->Use();
           SetUniformTex(scene_->GetActiveCamera(), model.get());
           model->RenderObject::Draw("Textured Triangles");
           BindLightUniforms(*shaders_["Textured Triangles"]);
        }
        else if((scene_->Toggle("Colors") || scene_->Toggle("Emissive") )&& model->HasLayout("Colored Triangles")  ){
            shaders_["Colored Triangles"]->Use();
            SetUniform(scene_->GetActiveCamera(), model.get());
            BindLightUniforms(*shaders_["Colored Triangles"]);
            model->RenderObject::Draw("Colored Triangles");
        }

        glDisable(GL_DEPTH_TEST);

        if(scene_->Toggle("Wireframe")) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if(scene_->Toggle("Normals") && model->HasLayout("Normals Triangles") ){
            shaders_["Normals Triangles"]->Use();
            SetUniformNormal(static_cast<MeshModel*>(model.get()), scene_->GetActiveCamera());
            model->RenderObject::Draw("Normals Triangles");
        }

        if(scene_->Toggle("Bounding Box")){
            DrawBoundingBox(static_cast<MeshModel*>(model.get()), scene_->GetActiveCamera());
        }
    }

    glEnable(GL_DEPTH_TEST);
    if(scene_->Toggle("Show Cameras")){
        shaders_["Basic"]->Use();
        for(const auto& cam : scene_->cameras_){
            if(cam.get()!=scene_->GetActiveCamera())
                DrawCamera(cam.get(), scene_->GetActiveCamera());
        }
        shaders_["Basic"]->Unuse();
    }
    glDisable(GL_DEPTH_TEST);

    if(scene_->Toggle("Show Frustum")){
        shaders_["Basic"]->Use();
        for(const auto& cam : scene_->cameras_){
            if(cam.get()!=scene_->GetActiveCamera())
                DrawFrustum(cam.get(), scene_->GetActiveCamera());;
        }
        shaders_["Basic"]->Unuse();
    }

    glEnable(GL_DEPTH_TEST);
    if(scene_->Toggle("Show Lights")){
        shaders_["Basic"]->Use();
        for(const auto& light : scene_->lights_){
            if(light->Type()!=LightSource::AMBIENT)
                DrawLight(light.get(), scene_->GetActiveCamera());;
        }
        shaders_["Basic"]->Unuse();
    }
    glDisable(GL_DEPTH_TEST);


    kipod::RenderManager::Bind(0);
    //framebuffer_->tex_->Draw();
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//SHADER  SETUP                             ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void OpenGLScene::SetupShaders(){
    SetupShaderTexturedTriangles();
    SetupShaderColoredTriangles();
    SetupShaderNormals();
    SetupShaderBasic();

}


void OpenGLScene::SetupShaderTexturedTriangles()
{
    shaders_.insert({"Textured Triangles", std::make_shared<kipod::Shader>("textures.vert.glsl", "textures.frag.glsl")});

    shaders_["Textured Triangles"]->AttachUniform<float>("tex");

        LOG_ENGINE("Attaching Uniforms to Texture Shader");
        shaders_["Textured Triangles"]->AttachUniform<glm::mat4>("v");
        shaders_["Textured Triangles"]->AttachUniform<glm::mat4>("mv");
        shaders_["Textured Triangles"]->AttachUniform<glm::mat4>("mv_normal");
        shaders_["Textured Triangles"]->AttachUniform<glm::mat4>("projection");

        AttachMaterialToShader(*shaders_["Textured Triangles"]);

        shaders_["Textured Triangles"]->AttachUniform<glm::vec4>("cameraLocation");

        for(int i = 0; i<3; ++i){
            AttachLightToShader(*shaders_["Textured Triangles"], i);
        }
}

void OpenGLScene::SetupShaderColoredTriangles()
{
    shaders_.insert({"Colored Triangles", std::make_shared<kipod::Shader>("lights.vert.glsl",   "lights.frag.glsl")});

    shaders_["Colored Triangles"]->AttachUniform<float>("tex");

        LOG_ENGINE("Attaching Uniforms to Shader Colored Triangles");
        shaders_["Colored Triangles"]->AttachUniform<glm::mat4>("view_matrix");
        shaders_["Colored Triangles"]->AttachUniform<glm::mat4>("mvp");
        shaders_["Colored Triangles"]->AttachUniform<glm::mat4>("model_matrix");
        shaders_["Colored Triangles"]->AttachUniform<glm::mat4>("mv_normal");
        shaders_["Colored Triangles"]->AttachUniform<glm::mat4>("projection");

        AttachMaterialToShader(*shaders_["Colored Triangles"]);

        shaders_["Colored Triangles"]->AttachUniform<glm::vec4>("camera_location");

        for(int i = 0; i<3; ++i){
            AttachLightToShader(*shaders_["Colored Triangles"], i);
        }
        shaders_["Colored Triangles"]->AttachUniform<int>("EmissiveOn");
}

void OpenGLScene::SetupShaderNormals()
{
    shaders_.insert({"Normals Triangles", std::make_shared<kipod::Shader>("normals.vert.glsl", "normals.frag.glsl", "normals.geom.glsl")});
    auto shader = shaders_["Normals Triangles"];
    shader->AttachUniform<float>("normal_length");
    shader->AttachUniform<glm::mat4>("mv");
    shader->AttachUniform<glm::mat4>("mv_normal");
    shader->AttachUniform<glm::mat4>("projection");
}

void OpenGLScene::SetupShaderBasic()
{
    shaders_.insert({"Basic", std::make_shared<kipod::Shader>("basic.vert.glsl",   "basic.frag.glsl")});
    shaders_["Basic"]->AttachUniform<glm::mat4>("mvp");
    shaders_["Basic"]->AttachUniform<glm::vec4>("color");
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//SETTING UNIFORMS                             ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void OpenGLScene::SetUniform(kipod::RenderCamera* camera, kipod::RenderObject* model)
{
    auto shader = shaders_["Colored Triangles"];
    BindMatrixUniforms(*shader, *model, *camera);
    BindMaterialUniforms(*shader, *(model->mat_));
}

void OpenGLScene::SetUniformNormal(MeshModel* model, kipod::RenderCamera* camera)
{
    auto shader = shaders_["Normals Triangles"];
    BindMatrixUniforms(*shader, *model, *camera);
    BindNormalUniforms(*shader, model->normal_length);
}

void OpenGLScene::SetUniformTex(kipod::RenderCamera* camera, kipod::RenderObject* model)
{
   auto shader = shaders_["Textured Triangles"];
   BindTextureUniforms(*shader, model->tex_.get());
   BindMatrixUniforms(*shader, *model, *camera);
   BindMaterialUniforms(*shader, *(model->mat_));
}

void OpenGLScene::SetUniformBox(MeshModel* model, kipod::RenderCamera* camera)
{
    glm::mat4 box = model->TansformBoundingBox();
    glm::mat4 p = camera->projection_matrix_;
    glm::mat4 v = camera->view_matrix_;
    glm::mat4 mv = v * box;

    shaders_["Colored Triangles"]->SetUniform<glm::mat4>("mv", mv);
    shaders_["Colored Triangles"]->SetUniform<glm::mat4>("projection", p);
}

void OpenGLScene::SetUniformCamera(kipod::RenderCamera *cameraModel, kipod::RenderCamera *camera)
{
    glm::mat4 cam = glm::inverse(cameraModel->view_matrix_) * glm::scale(glm::mat4(1.0f),glm::vec3(0.2f));
    glm::mat4 pv = camera->projection_view_matrix_;
    glm::mat4 mvp = pv * cam;
    shaders_["Basic"]->SetUniform<glm::mat4>("mvp", mvp);
    shaders_["Basic"]->SetUniform<glm::vec4>("color", glm::vec4(0.6,0.6,0.2,1));
}

void OpenGLScene::SetUniformFrustum(kipod::RenderCamera *cameraModel, kipod::RenderCamera *camera)
{

    glm::mat4 pv = camera->projection_view_matrix_;
    glm::mat4 vp = glm::inverse(cameraModel->projection_view_matrix_);
    auto mvp = pv * vp;
    shaders_["Basic"]->SetUniform<glm::mat4>("mvp", mvp);
    shaders_["Basic"]->SetUniform<glm::vec4>("color", glm::vec4(1,1,1,1));
}

void OpenGLScene::SetUniformLight(kipod::RenderLight *light, kipod::RenderCamera *camera)
{
    auto source = glm::vec3(light->Source().x, light->Source().y,light->Source().z);
    glm::mat4 cam = glm::translate(glm::scale(glm::mat4(1.0f),glm::vec3(0.2f)), source);
    glm::mat4 pv = camera->projection_view_matrix_;
    glm::mat4 mvp = pv * cam;
    shaders_["Basic"]->SetUniform<glm::mat4>("mvp", mvp);
    shaders_["Basic"]->SetUniform<glm::vec4>("color", light->Color());
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING UNIFORMS                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void OpenGLScene::BindLightUniforms(kipod::Shader& shader)
{
    for(int i = 0; i<3; ++i){
            SetLightToShader(shader, i, scene_->lights_[i].get());
    }
    shaders_["Colored Triangles"]->SetUniform<int>("EmissiveOn", scene_->Toggle("Emissive") );
}

kipod::GLRenderLayout&& OpenGLScene::CreateLayoutNormals(kipod::GLRenderLayout* layout)
{
    auto normal_layout = new kipod::GLRenderLayout(*layout);
    auto ebo = std::make_shared<kipod::ElementsBuffer>(*normal_layout->ebo_);
    ebo->primitive_ = GL_POINTS;
    normal_layout->ebo_ = ebo;
    normal_layout->sha_ = shaders_["Normals Triangles"];
    return std::move(*normal_layout);
}

void OpenGLScene::BindMaterialUniforms(kipod::Shader& shader, const kipod::RenderMaterial &material)
{
    SetMaterialToShader(shader, material);
}

void OpenGLScene::BindTextureUniforms(kipod::Shader& shader, const kipod::Texture* texture)
{
    shader.SetUniform<float>(texture->name_.c_str(), 0.0f);
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();
}

void OpenGLScene::BindNormalUniforms(kipod::Shader& shader, const float length)
{
    shader.SetUniform<float>("normal_length", length);
}

void OpenGLScene::BindMatrixUniforms(kipod::Shader& shader, const kipod::RenderObject& model, const kipod::RenderCamera& camera)
{
    glm::vec4 camLocation = glm::vec4(camera.eye_,1);
    glm::mat4 p = camera.projection_matrix_;
    glm::mat4 v = camera.view_matrix_;

    shader.SetUniform<glm::vec4>("camera_location", camLocation);
    shader.SetUniform<glm::mat4>("view_matrix", v);
    shader.SetUniform<glm::mat4>("projection", p);

    glm::mat4 m = model.Transform();

    glm::mat4 mvp = camera.projection_view_matrix_ * m;
    glm::mat4 mv_normal = glm::transpose(glm::inverse(v*m));

    shader.SetUniform<glm::mat4>("model_matrix", m);
    shader.SetUniform<glm::mat4>("mvp", mvp);
    shader.SetUniform<glm::mat4>("mv_normal", mv_normal);

}




/////////////////////////////////////////////////////////////////////////////////////////////////
//   MODEL CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void OpenGLScene::CreateMeshModelLayout(MeshModel *model)
{
    std::string name;
    bool foundTexture;
    name = model->tex_ ? (LOG_ENGINE("A Texture was set. Use Tex Shader"), foundTexture=true,  "Textured Triangles")
                       : (LOG_ENGINE("No Texture set. Use Light Shader."), foundTexture=false, "Colored Triangles" );
    auto layout = new kipod::GLRenderLayout;
    layout->sha_ = shaders_[name];
    model->AddLayout(name, std::move(*layout));
    model->Init(foundTexture);

    if(!foundTexture){
        model->AddLayout("Normals Triangles",
                         CreateLayoutNormals(
                             static_cast<kipod::GLRenderLayout*>(model->Layout(name))
                             ));
    }
}

void OpenGLScene::CreatePrimitiveModelLayout(PrimMeshModel* model)
{
    std::string name = "Colored Triangles";
    auto layout = new kipod::GLRenderLayout;
    layout->sha_ = shaders_["Colored Triangles"];
    model->AddLayout(name, std::move(*layout));

    model->Init(false);
    model->AddLayout("Normals Triangles",
                     CreateLayoutNormals(
                         static_cast<kipod::GLRenderLayout*>(model->Layout(name))
                         ));
}


//void OpenGLScene::LookAtModel(int camera_id, int model_id){
//    if(NumberOfModels() <= model_id) return;
//    if(NumberOfCameras() <= camera_id) return;
//    kipod::RenderCamera* cam =GetActiveCamera();
//    MeshModel* model = GetActiveModel();
//    cam->UpdateAt(model->Center());
//}









/////////////////////////////////////////////////////////////////////////////////////////////////
//COORDINATE AXIS, GRID, BOUNDING BOX, CAMERA                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////






void OpenGLScene::DrawBoundingBox(MeshModel* model, RenderCamera* camera)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    SetUniformBox(model,camera);
    scene_->bounding_box_.RenderObject::Draw("Colored Triangles");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OpenGLScene::DrawCamera(RenderCamera* camera_model, RenderCamera* camera)
{
    SetUniformCamera(camera_model, camera);
    scene_->bounding_box_.RenderObject::Draw("Basic");
}

void OpenGLScene::DrawFrustum(RenderCamera* camera_model, RenderCamera* camera)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    SetUniformFrustum(camera_model, camera);
    scene_->bounding_box_.RenderObject::Draw("Basic");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OpenGLScene::DrawLight(RenderLight* light, RenderCamera* camera)
{
    SetUniformLight(light, camera);
    scene_->bounding_sphere_.RenderObject::Draw("Basic");
}

void OpenGLScene::DrawCoordinateAxis(kipod::RenderCamera* camera)
{
    auto layout = static_cast<kipod::GLRenderLayout*>(scene_->coordinate_axis_->Layout("Coordinate Axis"));
    layout->sha_->Use();
    layout->sha_->SetUniform<glm::mat4>("mvp", *camera);
    layout->Draw();
    layout->sha_->Unuse();
}

void OpenGLScene::DrawGrid(kipod::RenderCamera* camera)
{
    auto layout =
    static_cast<kipod::GLRenderLayout*>(scene_->grid_->Layout("Grid")); layout->sha_->Use();
    layout->sha_->SetUniform<glm::mat4>("mvp", *camera); layout->Draw();
    layout->sha_->Unuse();
}


void OpenGLScene::CreateCoordinateAxisLayout(std::vector<vec3>& vertices,
                                                      std::vector<vec3>& colors)
{
    std::string name = "Coordinate Axis";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupLines(&vertices, &colors);
    layout->sha_ = std::make_shared<kipod::Shader>("lines.vert.glsl", "lines.frag.glsl");
    layout->sha_->AttachUniform<glm::mat4>("mvp");
    scene_->coordinate_axis_->AddLayout(name, std::move(*layout));
}

void OpenGLScene::CreateGridLayout(std::vector<vec3>& vertices)
{
    std::string name = "Grid";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupGrid(&vertices);
    layout->sha_ = std::make_shared<kipod::Shader>("grid.vert.glsl", "grid.frag.glsl");
    layout->sha_->AttachUniform<glm::mat4>("mvp");
    scene_->grid_->AddLayout(name, std::move(*layout));
}


}
