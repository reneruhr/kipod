#include "meshmodel_opengl_scene.h"
#include "../meshmodel_scene.h"
namespace kipod::MeshModels{



/////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void MeshModelOpenGLScene::Setup()
{
    SetupShaders();
}

void MeshModelOpenGLScene::CreateBoundingBoxLayout(PrimMeshModel* bounding_box){
    std::string name = "Colored Triangles";
    auto layout = new kipod::GLRenderLayout;
    layout->sha_ = shaders_["Colored Triangles"];
    bounding_box->AddLayout(name,std::move(*layout));
    bounding_box->Init(false,false);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//DRAWING                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelOpenGLScene::Draw()
{
    scene_->framebuffer_->Bind();
    glViewport(0, 0, scene_->width_, scene_->height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    DrawCoordinateAxis(scene_->coordinate_axis_.get(), scene_->GetActiveCamera());
    DrawGrid(scene_->grid_.get(), scene_->GetActiveCamera());
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
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            SetUniformBox(static_cast<MeshModel*>(model.get()),scene_->GetActiveCamera());
            scene_->bounding_box_.RenderObject::Draw("Colored Triangles");
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

//    if(scene_->Toggle("Camera Mode")){
//        shaders_["Basic"]->Use();
//        for(auto cam : scene_->cameras){
//            glm::mat4 pv = cam->projection_view_matrix_;
//            auto mvp = pv * glm::mat4(Translate(cam->getEye()));
//            shaders_["Basic"]->SetUniform<glm::mat4>("mvp", mvp);
//            cam->draw();
//        }
//    }
//    if(Toggle("Camera Frustum Mode")){
//        shaders_["Basic"]->Use();
//        for(auto cam : cameras){
//            glm::mat4 pv = cam->projection_view_matrix_;
//            glm::mat4 v = cam->view_matrix_;
//            glm::mat4 p = cam->projection_matrix_;
//            auto mvp = pv * glm::inverse(v) * glm::inverse(p);
//            shaders_["Basic"]->SetUniform<glm::mat4>("mvp", mvp);
//            cam->drawFrustum();
//        }
    kipod::RenderManager::Bind(0);

    //framebuffer_->tex_->Draw();
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//SHADER  SETUP                             ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelOpenGLScene::SetupShaders(){
    SetupShaderTexturedTriangles();
    SetupShaderColoredTriangles();
    SetupShaderNormals();
    void SetupShaderBasic();

}


void MeshModelOpenGLScene::SetupShaderTexturedTriangles()
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

void MeshModelOpenGLScene::SetupShaderColoredTriangles()
{
    shaders_.insert({"Colored Triangles", std::make_shared<kipod::Shader>("lights.vert.glsl",   "lights.frag.glsl")});

    shaders_["Colored Triangles"]->AttachUniform<float>("tex");

        LOG_ENGINE("Attaching Uniforms to Shader Colored Triangles");
        shaders_["Colored Triangles"]->AttachUniform<glm::mat4>("v");
        shaders_["Colored Triangles"]->AttachUniform<glm::mat4>("mv");
        shaders_["Colored Triangles"]->AttachUniform<glm::mat4>("mv_normal");
        shaders_["Colored Triangles"]->AttachUniform<glm::mat4>("projection");

        AttachMaterialToShader(*shaders_["Colored Triangles"]);

        shaders_["Colored Triangles"]->AttachUniform<glm::vec4>("cameraLocation");

        for(int i = 0; i<3; ++i){
            AttachLightToShader(*shaders_["Colored Triangles"], i);
        }
        shaders_["Colored Triangles"]->AttachUniform<int>("EmissiveOn");
}

void MeshModelOpenGLScene::SetupShaderNormals()
{
    shaders_.insert({"Normals Triangles", std::make_shared<kipod::Shader>("normals.vert.glsl", "normals.frag.glsl", "normals.geom.glsl")});
    auto shader = shaders_["Normals Triangles"];
    shader->AttachUniform<float>("normal_length");
    shader->AttachUniform<glm::mat4>("mv");
    shader->AttachUniform<glm::mat4>("mv_normal");
    shader->AttachUniform<glm::mat4>("projection");
}

void MeshModelOpenGLScene::SetupShaderBasic()
{
    shaders_.insert({"Basic", std::make_shared<kipod::Shader>("basic.vert.glsl",   "basic.frag.glsl")});
    shaders_["Basic"]->AttachUniform<glm::mat4>("mvp");
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//SETTING UNIFORMS                             ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelOpenGLScene::SetUniform(kipod::RenderCamera* camera, kipod::RenderObject* model)
{
    auto shader = shaders_["Colored Triangles"];
    BindMatrixUniforms(*shader, *model, *camera);
    BindMaterialUniforms(*shader, *(model->mat_));
}

void MeshModelOpenGLScene::SetUniformNormal(MeshModel* model, kipod::RenderCamera* camera)
{
    auto shader = shaders_["Normals Triangles"];
    BindMatrixUniforms(*shader, *model, *camera);
    BindNormalUniforms(*shader, model->normal_length);
}

void MeshModelOpenGLScene::SetUniformTex(kipod::RenderCamera* camera, kipod::RenderObject* model)
{
   auto shader = shaders_["Textured Triangles"];
   BindTextureUniforms(*shader, model->tex_.get());
   BindMatrixUniforms(*shader, *model, *camera);
   BindMaterialUniforms(*shader, *(model->mat_));
}

void MeshModelOpenGLScene::SetUniformBox(MeshModel* model, kipod::RenderCamera* camera)
{
    glm::mat4 box = model->TansformBoundingBox();
    glm::mat4 p = camera->projection_matrix_;
    glm::mat4 v = camera->view_matrix_;
    glm::mat4 mv = v * box;

    shaders_["Colored Triangles"]->SetUniform<glm::mat4>("mv", mv);
    shaders_["Colored Triangles"]->SetUniform<glm::mat4>("projection", p);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING UNIFORMS                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void MeshModelOpenGLScene::BindLightUniforms(kipod::Shader& shader)
{
    for(int i = 0; i<3; ++i){
            SetLightToShader(shader, i, scene_->lights_[i].get());
    }
    shaders_["Colored Triangles"]->SetUniform<int>("EmissiveOn", scene_->Toggle("Emissive") );
}

kipod::GLRenderLayout&& MeshModelOpenGLScene::CreateLayoutNormals(kipod::GLRenderLayout* layout)
{
    auto normal_layout = new kipod::GLRenderLayout(*layout);
    auto ebo = std::make_shared<kipod::ElementsBuffer>(*normal_layout->ebo_);
    ebo->primitive_ = GL_POINTS;
    normal_layout->ebo_ = ebo;
    normal_layout->sha_ = shaders_["Normals Triangles"];
    return std::move(*normal_layout);
}

void MeshModelOpenGLScene::BindMaterialUniforms(kipod::Shader& shader, const kipod::RenderMaterial &material)
{
    SetMaterialToShader(shader, material);
}

void MeshModelOpenGLScene::BindTextureUniforms(kipod::Shader& shader, const kipod::Texture* texture)
{
    shader.SetUniform<float>(texture->name_.c_str(), 0.0f);
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();
}

void MeshModelOpenGLScene::BindNormalUniforms(kipod::Shader& shader, const float length)
{
    shader.SetUniform<float>("normal_length", length);
}

void MeshModelOpenGLScene::BindMatrixUniforms(kipod::Shader& shader, const kipod::RenderObject& model, const kipod::RenderCamera& camera)
{
    glm::vec4  camLocation = glm::vec4(camera.eye_,1);
    glm::mat4 p = camera.projection_matrix_;
    glm::mat4 v = camera.view_matrix_;

    shader.SetUniform<glm::vec4>("cameraLocation", camLocation);
    shader.SetUniform<glm::mat4>("v", v);
    shader.SetUniform<glm::mat4>("projection", p);

    auto m = model.Transform();

    glm::mat4  mv = v*m;
    glm::mat4 mv_normal = glm::transpose(glm::inverse(mv));

    shader.SetUniform<glm::mat4>("mv", mv);
    shader.SetUniform<glm::mat4>("mv_normal", mv_normal);

}




/////////////////////////////////////////////////////////////////////////////////////////////////
//   MODEL CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void MeshModelOpenGLScene::CreateMeshModelLayout(MeshModel *model)
{
    model->SetUniformMaterial();

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

void MeshModelOpenGLScene::CreatePrimitiveModelLayout(PrimMeshModel *model)
{
    model->SetUniformMaterial();


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


//void MeshModelOpenGLScene::LookAtModel(int camera_id, int model_id){
//    if(NumberOfModels() <= model_id) return;
//    if(NumberOfCameras() <= camera_id) return;
//    kipod::RenderCamera* cam =GetActiveCamera();
//    MeshModel* model = GetActiveModel();
//    cam->UpdateAt(model->Center());
//}









/////////////////////////////////////////////////////////////////////////////////////////////////
//COORDINATE AXIS AND GRID                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////




void MeshModelOpenGLScene::CreateCoordinateAxisLayout(RenderObject* coordinate_axis,
                                                      std::vector<vec3>& vertices,
                                                      std::vector<vec3>& colors)
{
    std::string name = "Coordinate Axis";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupLines(&vertices, &colors);
    layout->sha_ = std::make_shared<kipod::Shader>("lines.vert.glsl", "lines.frag.glsl");
    layout->sha_->AttachUniform<glm::mat4>("mvp");
    coordinate_axis->AddLayout(name, std::move(*layout));
}

void MeshModelOpenGLScene::DrawCoordinateAxis(RenderObject* coordinate_axis, kipod::RenderCamera* camera){
    auto layout = static_cast<kipod::GLRenderLayout*>(coordinate_axis->Layout("Coordinate Axis"));
    layout->sha_->Use();
    layout->sha_->SetUniform<glm::mat4>("mvp", *camera);
    layout->Draw();
    layout->sha_->Unuse();
}

void MeshModelOpenGLScene::CreateGridLayout(RenderObject* grid,
                                            std::vector<vec3>& vertices)
{
    std::string name = "Grid";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupGrid(&vertices);
    layout->sha_ = std::make_shared<kipod::Shader>("grid.vert.glsl", "grid.frag.glsl");
    layout->sha_->AttachUniform<glm::mat4>("mvp");
    grid->AddLayout(name, std::move(*layout));
}

void MeshModelOpenGLScene::DrawGrid(RenderObject* grid, kipod::RenderCamera* camera){
    auto layout =
    static_cast<kipod::GLRenderLayout*>(grid->Layout("Grid")); layout->sha_->Use();
    layout->sha_->SetUniform<glm::mat4>("mvp", *camera); layout->Draw();
    layout->sha_->Unuse();
}

}
