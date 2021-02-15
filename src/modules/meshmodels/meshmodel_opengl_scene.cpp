#include "meshmodel_opengl_scene.h"

#include "../../camera.h"


/////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void MeshModelOpenGLScene::Setup()
{
    LOG_ENGINE("Seting up MeshModel Scene.");

    name_ = "Mesh Model OpenGL";

    Camera* cam = new Camera(45, float(width_)/height_, 0.1f, 200.0);
    cam->createFrustum(); // Needed for very first Camera
    addCamera(cam);

    Light* light = new Light(LightSource::AMBIENT, vec4(0.0f), vec4(0.1, 0.1, 0.1, 1.0));
    addLight(light);
    Light* light1 = new Light(LightSource::DIFFUSE, vec4(10.0f,1.0f,0.0f,1.0f), vec4(0.2, 0.3, 0.6, 1.0));
    addLight(light1);
    Light* light2 = new Light(LightSource::SPECULAR, vec4(0.0f,1.0f,10.0f,1.0f), vec4(1.0f));
    addLight(light2);

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
    Add(kipod::ModeToggle("Normals Mode", false));
    Add(kipod::ModeToggle("Camera Mode", false));
    Add(kipod::ModeToggle("Camera Frustum Mode", false));
    Add(kipod::ModeToggle("Color Mode", true));
    Add(kipod::ModeToggle("Texture Mode", true));
    Add(kipod::ModeToggle("Box Mode", false));
    Add(kipod::ModeToggle("Wireframe Mode", false));
    Add(kipod::ModeToggle("Emissive Mode",false));
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    for(auto model : models){
        if(Toggle("Wireframe Mode")) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glEnable(GL_DEPTH_TEST);

        if( Toggle("Texture Mode") && model->HasLayout("Textured Triangles") ){
           shaders_["Textured Triangles"].Use();
           SetUniformTex(lights, cameras[activeCamera], model);
           model->RenderObject::Draw("Textured Triangles");
        }
        else if((Toggle("Color Mode") || Toggle("Emissive Mode") )&& model->HasLayout("Colored Triangles")  ){
            shaders_["Colored Triangles"].Use();
            SetUniform(lights, cameras[activeCamera], model);
            model->RenderObject::Draw("Colored Triangles");
        }

        glDisable(GL_DEPTH_TEST);

        if(Toggle("Wireframe Mode")) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if(Toggle("Normals Mode") && model->HasLayout("Normals Triangles") ){
            shaders_["Normals Triangles"].Use();
            SetUniformNormal(model, cameras[activeCamera]);
            model->RenderObject::Draw("Normals Triangles");
        }

        if(Toggle("Box Mode")){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            SetUniformBox(model);
            boundingBox.RenderObject::Draw("Colored Triangles");
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

    }

    if(Toggle("Camera Mode")){
        shaders_["Basic"].Use();
        for(auto cam : cameras){
            glm::mat4 pv = cam->projection_view_matrix_;
            auto mvp = pv * glm::mat4(Translate(cam->getEye()));
            shaders_["Basic"].SetUniform<glm::mat4>("mvp", mvp);
            cam->draw();
        }
    }
    if(Toggle("Camera Frustum Mode")){
        shaders_["Basic"].Use();
        for(auto cam : cameras){
            glm::mat4 pv = cam->projection_view_matrix_;
            glm::mat4 v = cam->view_matrix_;
            glm::mat4 p = cam->projection_matrix_;
            auto mvp = pv * glm::inverse(v) * glm::inverse(p);
            shaders_["Basic"].SetUniform<glm::mat4>("mvp", mvp);
            cam->drawFrustum();
        }
    }

    glEnable(GL_DEPTH_TEST);

    DrawCoordinateAxis(cameras[activeCamera]);
    DrawGrid(cameras[activeCamera]);

    glDisable(GL_DEPTH_TEST);
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
//QUASICRYSTAL UNIFORMS                             ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////
//SETTING UNIFORMS                             ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelOpenGLScene::SetUniform(vector<Light*>& lights, Camera* camera, MeshModel* model)
{
    auto shader = shaders_["Colored Triangles"];
    BindLightUniforms(shader, lights);
    BindMatrixUniforms(shader, *model, *camera);
    BindMaterialUniforms(shader, *(model->mat_));
}

void MeshModelOpenGLScene::SetUniformNormal(MeshModel* model, Camera* camera)
{
    kipod::Shader* shader = &shaders_["Normals Triangles"];
    BindMatrixUniforms(*shader, *model, *camera);
    BindNormalUniforms(*shader, model->normal_length);
}

void MeshModelOpenGLScene::SetUniformTex(vector<Light*>& lights, Camera* camera, MeshModel* model)
{
   kipod::Shader* shader = &shaders_["Textured Triangles"];
   BindTextureUniforms(*shader, model->tex_);
   BindLightUniforms(*shader, lights);
   BindMatrixUniforms(*shader, *model, *camera);
   BindMaterialUniforms(*shader, *(model->mat_));
}

void MeshModelOpenGLScene::SetUniformBox(MeshModel* model)
{
    glm::mat4 box = model->getmTransformBBox();
    mat4 camc = cameras[activeCamera]->getcTransform();
    glm::mat4 mv = MakeGLM(camc) * box;
    glm::mat4 p = cameras[activeCamera]->projection_matrix_;

    shaders_["Colored Triangles"].SetUniform<glm::mat4>("mv", mv);
    shaders_["Colored Triangles"].SetUniform<glm::mat4>("projection", p);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//BINDING UNIFORMS                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void MeshModelOpenGLScene::BindLightUniforms(kipod::Shader& shader, vector<Light *> &lights)
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

void MeshModelOpenGLScene::BindMatrixUniforms(kipod::Shader& shader, const kipod::RenderObject& model, const Camera& camera)
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





void MeshModelOpenGLScene::loadOBJModel(string fileName, bool textured)
{
    MeshModel *model = new MeshModel(fileName, textured);

    model->setUniformMaterial();
    models.push_back(model);

    AddModel(model);
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

}

void MeshModelOpenGLScene::loadPrimitive(Primitive primitive, int numberPolygons)
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
    Camera* cam =cameras[camera_id];
    MeshModel* model = models[model_id];
    cam->updateAt(model->getCenter());
}





/////////////////////////////////////////////////////////////////////////////////////////////////
//LIGHT  CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelOpenGLScene::addLight(Light *light)
{
    lights.push_back(light);
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//CAMERA  CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void MeshModelOpenGLScene::addCamera(Camera *cam)
{
    cameras.push_back(cam);
}

void MeshModelOpenGLScene::setLastCameraActive()
{
    if(cameras.empty()) return;
    activeCamera = cameras.size()-1;
}

Camera* MeshModelOpenGLScene::getActiveCamera()
{
    return cameras[activeCamera];
}

MeshModel* MeshModelOpenGLScene::getActiveModel(){
    if(activeModel>=0) return models[activeModel];
    else return nullptr;
}

void MeshModelOpenGLScene::moveCamera(int camera_id, const vec3& eye, const vec3& at, const vec3& up )
{
    if(numberOfCameras() <= camera_id) return;
    Camera* cam = cameras[camera_id];
    cam->updateLookAt( eye, at, up );
}

void MeshModelOpenGLScene::moveEyeOfCamera(int camera_id, const vec3& eye)
{
    if(numberOfCameras() <= camera_id) return;
    Camera* cam = cameras[camera_id];
    cam->moveEye(eye);
}

void MeshModelOpenGLScene::perspectiveCamera(int camera_id, const float& fovy, const float& aspect, const float& near, const float& far)
{
    if(numberOfCameras() <= camera_id) return;
    Camera* cam =cameras[camera_id];
    cam->Perspective(fovy, aspect, near, far);
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//EVENTS                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelOpenGLScene::ProcessKeys(kipod::KeyPressedEvent &event)
{
        float stepsize = 1.0f;
        auto key = event.GetKeyCode();


        //TOGGLES:
        if(key == Key::Space)
            Toggle("Wireframe Mode").Switch();
        else if(key == Key::N)
            Toggle("Normals Mode").Switch();
        else if(key == Key::B)
            Toggle("Box Mode").Switch();
        else if(key == Key::C)
            Toggle("Camera Mode").Switch();
        else if(key == Key::X)
            Toggle("Clipping Mode").Switch();
        //TRANSFORM CONTROL:
        else if(key == Key::Left){
                if(getActiveModel())
                    getActiveModel()->world_->Translate({-stepsize,0,0});
                }
        else if(key == Key::Right){
                if(getActiveModel())
                    getActiveModel()->world_->Translate({+stepsize,0,0});
                }
        else if(key == Key::Up){
                if(getActiveModel())
                    getActiveModel()->world_->Translate({0,stepsize,0});
                }
        else if(key == Key::Down){
                if(getActiveModel())
                    getActiveModel()->world_->Translate({0,-stepsize,0});
                }
        else if(key == Key::PageUp){
                if(getActiveModel())
                    getActiveModel()->world_->Translate({0,0, stepsize});
                }
        else if(key == Key::PageDown){
                if(getActiveModel())
                    getActiveModel()->world_->Translate({0,0,-stepsize});
                }
        //CAMERA CONTROL:
        else if(key == Key::A){
                    getActiveCamera()->Move({-stepsize,0,0});
                }
        else if(key == Key::D){
                    getActiveCamera()->Move({+stepsize,0,0});
                }
        else if(key == Key::W){
                    getActiveCamera()->Move({0,stepsize,0});
                }
        else if(key == Key::S){
                    getActiveCamera()->Move({0,-stepsize,0});
                }
        else if(key == Key::R){
                    getActiveCamera()->Move({0,0, stepsize});
                }
        else if(key == Key::F){
                    getActiveCamera()->Move({0,0,-stepsize});
                }

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

