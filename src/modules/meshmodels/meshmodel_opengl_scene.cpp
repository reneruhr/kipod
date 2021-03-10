#include "meshmodel_opengl_scene.h"
namespace kipod::MeshModels{



/////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void MeshModelOpenGLScene::Setup()
{
    name_ = "Mesh Model OpenGL";

    AddCamera(kipod::RenderCamera({0.0f,0.0f,3.0f}));


    AddLight({kipod::LightSource::AMBIENT, vec4(0.0f), vec4(0.1, 0.1, 0.1, 1.0)});
    AddLight({kipod::LightSource::DIFFUSE, vec4(10.0f,5.0f,0.0f,1.0f), vec4(0.2, 0.3, 0.6, 1.0)});
    AddLight({kipod::LightSource::SPECULAR, vec4(5.0f,10.0f,0.0f,1.0f), vec4(1.0f)});
    SetActiveCamera(0);

    SetupShaders();

    std::string name = "Colored Triangles";
    auto layout = new kipod::GLRenderLayout;
    layout->sha_ = shaders_["Colored Triangles"];
    boundingBox.AddLayout(name,std::move(*layout));
    boundingBox.Init(false,false);

    SetupCoordinateAxis();
    SetupGrid();

    SetupOptions();
    LOG_ENGINE("MeshModel Scene Initialized.");
    LOG_CONSOLE("MeshModel Scene Initialized.");
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
    glViewport(0, 0, width_, height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    DrawCoordinateAxis(GetActiveCamera());
    DrawGrid(GetActiveCamera());
    glDisable(GL_DEPTH_TEST);

    for(const auto& model : render_objects_){
        if(Toggle("Wireframe")) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glEnable(GL_DEPTH_TEST);

        if( Toggle("Textures") && model->HasLayout("Textured Triangles") ){
           shaders_["Textured Triangles"]->Use();
           SetUniformTex(GetActiveCamera(), model.get());
           model->RenderObject::Draw("Textured Triangles");
           BindLightUniforms(*shaders_["Textured Triangles"]);
        }
        else if((Toggle("Colors") || Toggle("Emissive") )&& model->HasLayout("Colored Triangles")  ){
            shaders_["Colored Triangles"]->Use();
            SetUniform(GetActiveCamera(), model.get());
            BindLightUniforms(*shaders_["Colored Triangles"]);
            model->RenderObject::Draw("Colored Triangles");
        }

        glDisable(GL_DEPTH_TEST);

        if(Toggle("Wireframe")) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if(Toggle("Normals") && model->HasLayout("Normals Triangles") ){
            shaders_["Normals Triangles"]->Use();
            SetUniformNormal(static_cast<MeshModel*>(model.get()), GetActiveCamera());
            model->RenderObject::Draw("Normals Triangles");
        }

        if(Toggle("Bounding Box")){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            SetUniformBox(static_cast<MeshModel*>(model.get()),GetActiveCamera());
            boundingBox.RenderObject::Draw("Colored Triangles");
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

    }

//    if(Toggle("Camera Mode")){
//        shaders_["Basic"]->Use();
//        for(auto cam : cameras){
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
//    }

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
            SetLightToShader(shader, i, lights_[i].get());
    }
    shaders_["Colored Triangles"]->SetUniform<int>("EmissiveOn", Toggle("Emissive") );
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




void MeshModelOpenGLScene::SetActiveModel(int id){
    SetActiveRenderObject(id);
}

MeshModel* MeshModelOpenGLScene::GetActiveModel(){
    if(HasModel()) return static_cast<MeshModel*>(GetActiveRenderObject());
    else return nullptr;
}

void MeshModelOpenGLScene::AddModel(MeshModel && model)
{
    render_objects_.push_back(
                std::make_unique<MeshModel>(
                    std::forward<MeshModel>(model)));
}

void MeshModelOpenGLScene::LoadOBJModel(std::filesystem::path path, bool textured)
{
    MeshModel *model = new MeshModel(path, textured);
    if(!model->Valid()) return;
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

    AddModel(std::move(*model));
}

void MeshModelOpenGLScene::LoadPrimitive(Primitive primitive, int numberPolygons)
{
    PrimMeshModel *model = new PrimMeshModel(primitive, numberPolygons);
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

    AddModel(std::move(*model));
}


//void MeshModelOpenGLScene::LookAtModel(int camera_id, int model_id){
//    if(NumberOfModels() <= model_id) return;
//    if(NumberOfCameras() <= camera_id) return;
//    kipod::RenderCamera* cam =GetActiveCamera();
//    MeshModel* model = GetActiveModel();
//    cam->UpdateAt(model->Center());
//}







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
        else if(key == Key::A && mod == Mod::KIPOD_MOD_CONTROL){
                    GetActiveCamera()->Move({-stepsize,0,0});
                }
        else if(key == Key::D && mod == Mod::KIPOD_MOD_CONTROL){
                    GetActiveCamera()->Move({+stepsize,0,0});
                }
        else if(key == Key::W && mod == Mod::KIPOD_MOD_CONTROL){
                    GetActiveCamera()->Move({0,stepsize,0});
                }
        else if(key == Key::S && mod == Mod::KIPOD_MOD_CONTROL){
                    GetActiveCamera()->Move({0,-stepsize,0});
                }
        else if(key == Key::R && mod == Mod::KIPOD_MOD_CONTROL){
                    GetActiveCamera()->Move({0,0, stepsize});
                }
        else if(key == Key::F && mod == Mod::KIPOD_MOD_CONTROL){
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
    kipod::Events::Signup(this, kipod::EventCategoryKeyboard); LOG_CATEGORY_NAME(kipod::EventCategoryKeyboard);
    kipod::Events::Signup(this, kipod::EventCategoryMouseButton); LOG_CATEGORY_NAME(kipod::EventCategoryMouseButton);
    kipod::Events::Signup(this, kipod::EventCategoryMouse); LOG_CATEGORY_NAME(kipod::EventCategoryMouse);
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
    layout->sha_ = std::make_shared<kipod::Shader>("lines.vert.glsl", "lines.frag.glsl");
    layout->sha_->AttachUniform<glm::mat4>("mvp");
    coordinate_axis_->AddLayout(name, std::move(*layout));
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

    int n=100;
    for(int i=-n; i<=n; ++i){
        vertices.push_back(vec3(-n,i,0));
        vertices.push_back(vec3(n,i,0));
        vertices.push_back(vec3(i,-n,0));
        vertices.push_back(vec3(i,n,0));
    }

    std::string name = "Grid";
    auto layout = new kipod::GLRenderLayout;
    layout->SetupGrid(&vertices);
    layout->sha_ = std::make_shared<kipod::Shader>("grid.vert.glsl", "grid.frag.glsl");
    layout->sha_->AttachUniform<glm::mat4>("mvp");
    grid_->AddLayout(name, std::move(*layout));
}

void MeshModelOpenGLScene::DrawGrid(kipod::RenderCamera* camera){
    auto layout = static_cast<kipod::GLRenderLayout*>(grid_->Layout("Grid"));
    layout->sha_->Use();
    layout->sha_->SetUniform<glm::mat4>("mvp", *camera);
    layout->Draw();
    layout->sha_->Unuse();
}

}
