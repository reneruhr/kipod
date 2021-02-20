#include "meshmodel_opengl_scene.h"





/////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void MeshModelOpenGLScene::Setup()
{
    LOG_ENGINE("Seting up MeshModel Scene.");

    name_ = "Mesh Model OpenGL";

    kipod::RenderCamera* cam = new kipod::RenderCamera({0.0f,0.0f,3.0f});
    AddCamera(cam);

    kipod::RenderLight* light = new kipod::RenderLight(kipod::LightSource::AMBIENT, vec4(0.0f), vec4(0.1, 0.1, 0.1, 1.0));
    AddLight(light);
    kipod::RenderLight* light1 = new kipod::RenderLight(kipod::LightSource::DIFFUSE, vec4(10.0f,1.0f,0.0f,1.0f), vec4(0.2, 0.3, 0.6, 1.0));
    AddLight(light1);
    kipod::RenderLight* light2 = new kipod::RenderLight(kipod::LightSource::SPECULAR, vec4(0.0f,1.0f,10.0f,1.0f), vec4(1.0f));
    AddLight(light2);

    SetupShaders();

    std::string name = "Colored Triangles";
    auto layout = new kipod::GLRenderLayout;
    layout->sha_ = &shaders_["Colored Triangles"];
    boundingBox.AddLayout(name,layout);
    boundingBox.Init(false,false);
    {
        auto normal_layout = new kipod::GLRenderLayout(*layout);
        auto ebo = new kipod::ElementsBuffer(*normal_layout->ebo_);
        ebo->primitive_ = GL_POINTS;
        normal_layout->ebo_ = ebo;
        normal_layout->sha_ = &shaders_["Normals Triangles"];
        boundingBox.AddLayout({"Normals Triangles", normal_layout});
    }

    SetupCoordinateAxis();
    SetupGrid();

    SetupOptions();
    Signup();
}

void MeshModelOpenGLScene::SetupOptions(){
    Add(kipod::ModeToggle("Normals", false));
    Add(kipod::ModeToggle("Camera Mode", false));
    Add(kipod::ModeToggle("Camera Frustum Mode", false));
    Add(kipod::ModeToggle("Colors", true));
    Add(kipod::ModeToggle("Textures", true));
    Add(kipod::ModeToggle("Bounding Box", false));
    Add(kipod::ModeToggle("Wireframe", false));
    Add(kipod::ModeToggle("Emissive",false));
    Add(kipod::ModeToggle("Clipping Mode", true));
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//DRAWING                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelOpenGLScene::Draw()
{
    framebuffer_->Bind();
    //kipod::RenderManager::Bind(0);
    glViewport(0, 0, width_, height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    DrawCoordinateAxis(cameras[activeCamera]);
    DrawGrid(cameras[activeCamera]);
    glDisable(GL_DEPTH_TEST);

    for(auto model : models){
        if(Toggle("Wireframe")) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glEnable(GL_DEPTH_TEST);

        if( Toggle("Textures") && model->HasLayout("Textured Triangles") ){
           shaders_["Textured Triangles"].Use();
           SetUniformTex(lights, cameras[activeCamera], model);
           model->RenderObject::Draw("Textured Triangles");
        }
        else if((Toggle("Colors") || Toggle("Emissive") )&& model->HasLayout("Colored Triangles")  ){
            shaders_["Colored Triangles"].Use();
            SetUniform(lights, cameras[activeCamera], model);
            model->RenderObject::Draw("Colored Triangles");
        }

        glDisable(GL_DEPTH_TEST);

        if(Toggle("Wireframe")) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if(Toggle("Normals") && model->HasLayout("Normals Triangles") ){
            shaders_["Normals Triangles"].Use();
            SetUniformNormal(model, cameras[activeCamera]);
            model->RenderObject::Draw("Normals Triangles");
        }

        if(Toggle("Bounding Box")){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            SetUniformBox(model,cameras[activeCamera]);
            boundingBox.RenderObject::Draw("Colored Triangles");
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

    }

//    if(Toggle("Camera Mode")){
//        shaders_["Basic"].Use();
//        for(auto cam : cameras){
//            glm::mat4 pv = cam->projection_view_matrix_;
//            auto mvp = pv * glm::mat4(Translate(cam->getEye()));
//            shaders_["Basic"].SetUniform<glm::mat4>("mvp", mvp);
//            cam->draw();
//        }
//    }
//    if(Toggle("Camera Frustum Mode")){
//        shaders_["Basic"].Use();
//        for(auto cam : cameras){
//            glm::mat4 pv = cam->projection_view_matrix_;
//            glm::mat4 v = cam->view_matrix_;
//            glm::mat4 p = cam->projection_matrix_;
//            auto mvp = pv * glm::inverse(v) * glm::inverse(p);
//            shaders_["Basic"].SetUniform<glm::mat4>("mvp", mvp);
//            cam->drawFrustum();
//        }
//    }

    kipod::RenderManager::Bind(0);
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
    shaders_.insert({"Textured Triangles", kipod::Shader("textures.vert.glsl", "textures.frag.glsl")});

    shaders_["Textured Triangles"].AttachUniform<float>("tex");

        LOG_ENGINE("Attaching Uniforms to Texture Shader");
        shaders_["Textured Triangles"].AttachUniform<glm::mat4>("v");
        shaders_["Textured Triangles"].AttachUniform<glm::mat4>("mv");
        shaders_["Textured Triangles"].AttachUniform<glm::mat4>("mv_normal");
        shaders_["Textured Triangles"].AttachUniform<glm::mat4>("projection");

        AttachMaterialToShader(shaders_["Textured Triangles"]);

        shaders_["Textured Triangles"].AttachUniform<glm::vec4>("cameraLocation");

        for(int i = 0; i<3; ++i){
            AttachLightToShader(shaders_["Textured Triangles"], i);
        }
}

void MeshModelOpenGLScene::SetupShaderColoredTriangles()
{
    shaders_.insert({"Colored Triangles", kipod::Shader("lights.vert.glsl",   "lights.frag.glsl")});

    shaders_["Colored Triangles"].AttachUniform<float>("tex");

        LOG_ENGINE("Attaching Uniforms to Shader Colored Triangles");
        shaders_["Colored Triangles"].AttachUniform<glm::mat4>("v");
        shaders_["Colored Triangles"].AttachUniform<glm::mat4>("mv");
        shaders_["Colored Triangles"].AttachUniform<glm::mat4>("mv_normal");
        shaders_["Colored Triangles"].AttachUniform<glm::mat4>("projection");

        AttachMaterialToShader(shaders_["Colored Triangles"]);

        shaders_["Colored Triangles"].AttachUniform<glm::vec4>("cameraLocation");

        for(int i = 0; i<3; ++i){
            AttachLightToShader(shaders_["Colored Triangles"], i);
        }
}

void MeshModelOpenGLScene::SetupShaderNormals()
{
    shaders_.insert({"Normals Triangles", kipod::Shader("normals.vert.glsl", "normals.frag.glsl", "normals.geom.glsl")});
    auto shader = shaders_["Normals Triangles"];
    shader.AttachUniform<float>("normal_length");
    shader.AttachUniform<glm::mat4>("mv");
    shader.AttachUniform<glm::mat4>("mv_normal");
    shader.AttachUniform<glm::mat4>("projection");
}

void MeshModelOpenGLScene::SetupShaderBasic()
{
    shaders_.insert({"Basic", kipod::Shader("basic.vert.glsl",   "basic.frag.glsl")});
    shaders_["Basic"].AttachUniform<glm::mat4>("mvp");
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//SETTING UNIFORMS                             ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelOpenGLScene::SetUniform(vector<kipod::RenderLight*>& lights, kipod::RenderCamera* camera, MeshModel* model)
{
    auto shader = shaders_["Colored Triangles"];
    BindLightUniforms(shader, lights);
    BindMatrixUniforms(shader, *model, *camera);
    BindMaterialUniforms(shader, *(model->mat_));
}

void MeshModelOpenGLScene::SetUniformNormal(MeshModel* model, kipod::RenderCamera* camera)
{
    kipod::Shader* shader = &shaders_["Normals Triangles"];
    BindMatrixUniforms(*shader, *model, *camera);
    BindNormalUniforms(*shader, model->normal_length);
}

void MeshModelOpenGLScene::SetUniformTex(vector<kipod::RenderLight*>& lights, kipod::RenderCamera* camera, MeshModel* model)
{
   kipod::Shader* shader = &shaders_["Textured Triangles"];
   BindTextureUniforms(*shader, model->tex_);
   BindLightUniforms(*shader, lights);
   BindMatrixUniforms(*shader, *model, *camera);
   BindMaterialUniforms(*shader, *(model->mat_));
}

void MeshModelOpenGLScene::SetUniformBox(MeshModel* model, kipod::RenderCamera* camera)
{
    glm::mat4 box = model->TansformBoundingBox();
    glm::mat4 p = camera->projection_matrix_;
    glm::mat4 v = camera->view_matrix_;
    glm::mat4 mv = v * box;

    shaders_["Colored Triangles"].SetUniform<glm::mat4>("mv", mv);
    shaders_["Colored Triangles"].SetUniform<glm::mat4>("projection", p);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING UNIFORMS                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void MeshModelOpenGLScene::BindLightUniforms(kipod::Shader& shader, vector<kipod::RenderLight *> &lights)
{
    for(int i = 0; i<3; ++i){
            SetLightToShader(shader, i, lights[i]);
    }
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




void MeshModelOpenGLScene::SetActiveModel(int id){
    if(id<numberOfModels()) activeModel = id;
}

MeshModel* MeshModelOpenGLScene::GetActiveModel(){
    if(activeModel>=0) return models[activeModel];
    else return nullptr;
}

void MeshModelOpenGLScene::LoadOBJModel(string fileName, bool textured)
{
    MeshModel *model = new MeshModel(fileName, textured);
    if(!model->Valid()) return;
    model->setUniformMaterial();

    std::string name;
    bool foundTexture;
    name = model->tex_ ? (LOG_ENGINE("A Texture was set. Use Tex Shader"), foundTexture=true,  "Textured Triangles")
                       : (LOG_ENGINE("No Texture set. Use Light Shader."), foundTexture=false, "Colored Triangles" );
    auto layout = new kipod::GLRenderLayout;
    layout->sha_ = &shaders_[name];
    LOG_ENGINE("Add Layout name {}", name);
    model->AddLayout({name, layout});
    model->Init(foundTexture);

    if(!foundTexture){
        auto normal_layout = new kipod::GLRenderLayout(*layout);
        auto ebo = new kipod::ElementsBuffer(*normal_layout->ebo_);
        ebo->primitive_ = GL_POINTS;
        normal_layout->ebo_ = ebo;
        normal_layout->sha_ = &shaders_["Normals Triangles"];
        model->AddLayout({"Normals Triangles", normal_layout});
    }

    models.push_back(model);
    AddModel(model);
}

void MeshModelOpenGLScene::LoadPrimitive(Primitive primitive, int numberPolygons)
{
    PrimMeshModel *model = new PrimMeshModel(primitive, numberPolygons);
    model->setUniformMaterial();
    models.push_back(model);

    AddModel(model);
    std::string name = "Colored Triangles";
    auto layout = new kipod::GLRenderLayout;
    layout->sha_ = &shaders_["Colored Triangles"];
    model->AddLayout({name, layout});
    model->Init(false);
    {
        auto normal_layout = new kipod::GLRenderLayout(*layout);
        auto ebo = new kipod::ElementsBuffer(*normal_layout->ebo_);
        ebo->primitive_ = GL_POINTS;
        normal_layout->ebo_ = ebo;
        normal_layout->sha_ = &shaders_["Normals Triangles"];
        model->AddLayout({"Normals Triangles", normal_layout});
    }
}



void MeshModelOpenGLScene::moveModel(int model_id, const vec3& translate){
    if(numberOfModels() <= model_id) return;
    MeshModel* model = models[model_id];
    model->move(translate);
}

void MeshModelOpenGLScene::lookAtModel(int camera_id, int model_id){
    if(numberOfModels() <= model_id) return;
    if(numberOfCameras() <= camera_id) return;
    kipod::RenderCamera* cam =cameras[camera_id];
    MeshModel* model = models[model_id];
    cam->UpdateAt(model->getCenter());
}





/////////////////////////////////////////////////////////////////////////////////////////////////
//LIGHT  CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelOpenGLScene::AddLight(kipod::RenderLight *light)
{
    lights.push_back(light);
}




/////////////////////////////////////////////////////////////////////////////////////////////////
//CAMERA  CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void MeshModelOpenGLScene::AddCamera(kipod::RenderCamera *cam)
{
    cameras.push_back(cam);
}
kipod::RenderCamera* MeshModelOpenGLScene::GetActiveCamera()
{
    return cameras[activeCamera];
}



void MeshModelOpenGLScene::SetActiveCamera(int id){
    if(id<numberOfCameras()) activeCamera = id;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//EVENTS                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelOpenGLScene::ProcessKeys(kipod::KeyPressedEvent &event)
{
        float stepsize = 1.0f;
        auto key = event.GetKeyCode();
        auto mod = event.GetMod();


        //TOGGLES:
        if(key == Key::Space)
            Toggle("Wireframe").Switch();
        else if(key == Key::N)
            Toggle("Normals").Switch();
        else if(key == Key::B)
            Toggle("Bounding Box").Switch();
        else if(key == Key::C)
            Toggle("Camera Mode").Switch();
        else if(key == Key::X)
            Toggle("Clipping Mode").Switch();
        //TRANSFORM CONTROL:
        else if(key == Key::Left){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({-stepsize,0,0});
                }
        else if(key == Key::Right){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({+stepsize,0,0});
                }
        else if(key == Key::Up){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({0,stepsize,0});
                }
        else if(key == Key::Down){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({0,-stepsize,0});
                }
        else if(key == Key::PageUp){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({0,0, stepsize});
                }
        else if(key == Key::PageDown){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({0,0,-stepsize});
                }
        //CAMERA CONTROL:
        else if(key == Key::A && mod == Mod::MOD_CONTROL){
                    GetActiveCamera()->Move({-stepsize,0,0});
                }
        else if(key == Key::D && mod == Mod::MOD_CONTROL){
                    GetActiveCamera()->Move({+stepsize,0,0});
                }
        else if(key == Key::W && mod == Mod::MOD_CONTROL){
                    GetActiveCamera()->Move({0,stepsize,0});
                }
        else if(key == Key::S && mod == Mod::MOD_CONTROL){
                    GetActiveCamera()->Move({0,-stepsize,0});
                }
        else if(key == Key::R && mod == Mod::MOD_CONTROL){
                    GetActiveCamera()->Move({0,0, stepsize});
                }
        else if(key == Key::F && mod == Mod::MOD_CONTROL){
                    GetActiveCamera()->Move({0,0,-stepsize});
                }

        else if(key == Key::A && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::LEFT, stepsize);
                }
        else if(key == Key::D && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::RIGHT, stepsize);
                }
        else if(key == Key::W && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::FORWARD, stepsize);
                }
        else if(key == Key::S && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::BACKWARD, stepsize);
                }
        else if(key == Key::R && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::UP, stepsize);
                }
        else if(key == Key::F && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::DOWN, stepsize);
                }

}

void MeshModelOpenGLScene::ProcessMouseButtons(kipod::MouseButtonEvent &event)
{
    auto button = event.GetButton();

    if(button == MouseButton::Button0 && event.GetEventType() == kipod::EventType::MouseButtonPressed)
        mouse_rotation_active_ = true;
    else if(button == MouseButton::Button0 && event.GetEventType() == kipod::EventType::MouseButtonReleased)
        mouse_rotation_active_ = false;
}

void MeshModelOpenGLScene::ProcessMouseMoves(kipod::MouseMoveEvent &event)
{
    if(mouse_rotation_active_)
        GetActiveCamera()->Rotate(event.x(),event.y());
}

void MeshModelOpenGLScene::Signup() {
    kipod::Events::Signup(*this, kipod::EventCategoryKeyboard); LOG_CATEGORY_NAME(kipod::EventCategoryKeyboard);
    kipod::Events::Signup(*this, kipod::EventCategoryMouseButton); LOG_CATEGORY_NAME(kipod::EventCategoryMouseButton);
    kipod::Events::Signup(*this, kipod::EventCategoryMouse); LOG_CATEGORY_NAME(kipod::EventCategoryMouseB);
}

void MeshModelOpenGLScene::Receive(std::shared_ptr<kipod::Event> event){
    Process<kipod::KeyPressedEvent>(event, BIND_EVENT_FN(MeshModelOpenGLScene::ProcessKeys));
    Process<kipod::MouseButtonPressEvent>(event, BIND_EVENT_FN(MeshModelOpenGLScene::ProcessMouseButtons));
    Process<kipod::MouseButtonReleaseEvent>(event, BIND_EVENT_FN(MeshModelOpenGLScene::ProcessMouseButtons));
    Process<kipod::MouseMoveEvent>(event, BIND_EVENT_FN(MeshModelOpenGLScene::ProcessMouseMoves));
}




/////////////////////////////////////////////////////////////////////////////////////////////////
//COORDINATE AXIS AND GRID                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////




void MeshModelOpenGLScene::SetupCoordinateAxis()
{
    coordinate_axis_ = std::make_unique<kipod::RenderObject>();

    std::vector<vec3> vertices = {vec3(-2,0,0),vec3(10,0,0),
                                  vec3(0,-2,0),vec3(0,10,0),
                                  vec3(0,0,-2),vec3(0,0,10),
                                 };
    std::vector<vec3> colors =   {vec3(1.0,0.5,0.5),vec3(1,0,0),
                                  vec3(0.5,1.0,0.5),vec3(0,1,0),
                                  vec3(0.5,0.5,1.0),vec3(0,0,1),
                                  };

    std::string name = "Coordinate Axis";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupLines(&vertices, &colors);
    layout->sha_ = new kipod::Shader("lines.vert.glsl", "lines.frag.glsl");
    layout->sha_->AttachUniform<glm::mat4>("mvp");
    coordinate_axis_->AddLayout(name, layout);
}

void MeshModelOpenGLScene::DrawCoordinateAxis(kipod::RenderCamera* camera){
    auto layout = static_cast<kipod::GLRenderLayout*>(coordinate_axis_->Layout("Coordinate Axis"));
    layout->sha_->Use();
    layout->sha_->SetUniform<glm::mat4>("mvp", *camera);
    layout->Draw();
    layout->sha_->Unuse();
}

void MeshModelOpenGLScene::SetupGrid()
{
    grid_ = std::make_unique<kipod::RenderObject>();

    std::vector<vec3> vertices;

    for(int i=-10; i<=10; ++i){
        vertices.push_back(vec3(-10,i,0));
        vertices.push_back(vec3(10,i,0));
        vertices.push_back(vec3(i,-10,0));
        vertices.push_back(vec3(i,10,0));
    }

    std::string name = "Grid";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupGrid(&vertices);
    layout->sha_ = new kipod::Shader("grid.vert.glsl", "grid.frag.glsl");
    layout->sha_->AttachUniform<glm::mat4>("mvp");
    grid_->AddLayout(name, layout);
}

void MeshModelOpenGLScene::DrawGrid(kipod::RenderCamera* camera){
    auto layout = static_cast<kipod::GLRenderLayout*>(grid_->Layout("Grid"));
    layout->sha_->Use();
    layout->sha_->SetUniform<glm::mat4>("mvp", *camera);
    layout->Draw();
    layout->sha_->Unuse();
}

