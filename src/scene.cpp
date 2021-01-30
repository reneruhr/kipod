#include "../include/core.h"
#include "../include/scene.h"
#include "../include/meshmodel.h"

#include "../include/quacry.h"
#include <glm/gtc/type_ptr.hpp>

using namespace std;



void Scene::Setup()
{
    LOG_ENGINE("Seting up MeshModel Scene.");
    SetupUniforms();

    std::string name = "Colored Triangles";
    auto layout = new kipod::GLRenderLayout;
    layout->sha_ = &shaders_["Colored Triangles"];
    boundingBox.AddLayout(name,layout);
    boundingBox.init(_glrenderer);
    boundingBox.Init(false);
    {
        auto normal_layout = new kipod::GLRenderLayout(*layout);
        auto ebo = new kipod::ElementsBuffer(*normal_layout->ebo_);
        ebo->primitive_ = GL_POINTS;
        normal_layout->ebo_ = ebo;
        normal_layout->sha_ = &shaders_["Normals Triangles"];
        boundingBox.AddLayout({"Normals Triangles", normal_layout});
    }

    framebuffer_ = new kipod::FrameBuffer(width_, height_); //kipod::RenderManager::addFrameBuffer(width_, height_);
    LOG_ENGINE("Scene Framebuffer id is {}", framebuffer_->opengl_id_);
}



void Scene::BindLightUniforms(kipod::Shader& shader, vector<Light *> &lights)
{
    for(int i = 0; i<3; ++i){
            SetLightToShader(shader, i, lights[i]);
    }
}

void Scene::BindMaterialUniforms(kipod::Shader& shader, const kipod::RenderMaterial &material)
{
    SetMaterialToShader(shader, material);
}

void Scene::BindTextureUniforms(kipod::Shader& shader, const kipod::Texture* texture)
{
    shader.SetUniform<float>(texture->name_.c_str(), 0.0f);
    glActiveTexture(GL_TEXTURE0);
    texture->Bind();
}

void Scene::BindNormalUniforms(kipod::Shader& shader, const float length)
{
    shader.SetUniform<float>("normal_length", length);
}

void Scene::BindMatrixUniforms(kipod::Shader& shader, const kipod::RenderObject& model, const Camera& camera)
{
    vec4 eye = vec4(camera.getEye());
    mat4 camp = camera.getProjection(camerasMode[activeCamera]);
    mat4 camc = camera.getcTransform();
    glm::vec4  camLocation = MakeGLM(eye);
    glm::mat4 p = MakeGLM(camp);
    glm::mat4 v = MakeGLM(camc);


        shader.SetUniform<glm::vec4>("cameraLocation", camLocation);
        shader.SetUniform<glm::mat4>("v", v);
        shader.SetUniform<glm::mat4>("projection", p);


    auto m = model.Transform();

    glm::mat4  mv = v*m;
    glm::mat4 mv_normal = glm::transpose(glm::inverse(mv));

        shader.SetUniform<glm::mat4>("mv", mv);
        shader.SetUniform<glm::mat4>("mv_normal", mv_normal);

}

void Scene::BindMatrixUniformsForMesh(kipod::Shader& shader, const MeshModel& model, const Camera& camera){
    vec4 eye = vec4(camera.getEye());
    mat4 camp = camera.getProjection(camerasMode[activeCamera]);
    mat4 camc = camera.getcTransform();
    glm::vec4  camLocation = MakeGLM(eye);
    glm::mat4 p = MakeGLM(camp);
    glm::mat4 v = MakeGLM(camc);


        shader.SetUniform<glm::vec4>("cameraLocation", camLocation);
        shader.SetUniform<glm::mat4>("v", v);
        shader.SetUniform<glm::mat4>("projection", p);


    //auto m = model.Transform();
    mat4 mm = camc*model.getmTransform();
    glm::mat4 mv = MakeGLM(mm);
    //glm::mat4  mv = v*m;
    glm::mat4 mv_normal = glm::transpose(glm::inverse(mv)); // order?
    //glm::mat4 mv_normal = glm::inverse(glm::transpose(mv)); // order?

        shader.SetUniform<glm::mat4>("mv", mv);
        shader.SetUniform<glm::mat4>("mv_normal", mv_normal);

}


void Scene::SetUniform(vector<Light*>& lights, Camera* camera, MeshModel* model)
{
    auto shader = shaders_["Colored Triangles"];
    BindLightUniforms(shader, lights);
    BindMatrixUniformsForMesh(shader, *model, *camera);
    BindMaterialUniforms(shader, *(model->mat_));
}

void Scene::SetUniformNormal(MeshModel* model, Camera* camera)
{
    auto shader = shaders_["Normals Triangles"];
    BindMatrixUniformsForMesh(shader, *model, *camera);
    BindNormalUniforms(shader, model->normal_length);
}

void Scene::SetUniformTex(vector<Light*>& lights, Camera* camera, MeshModel* model)
{
   auto shader = shaders_["Textured Triangles"];
   BindTextureUniforms(shader, model->tex_);
   BindLightUniforms(shader, lights);
   BindMatrixUniformsForMesh(shader, *model, *camera);
   BindMaterialUniforms(shader, *(model->mat_));
}



//void Scene::Draw()
//{
//    kipod::RenderManager::Bind(0);
//    BindLightUniforms(lights);
//    shaders_["Textured Triangles"].SetUniform<float>("tex", 0.0f);
//    for(auto& model : render_objects_){
//        BindMatrixUniforms(*model, *cameras[activeCamera]);
//        BindMaterialUniforms(*(model->mat_));
//        model->Draw();
//    }
//}



void Scene::loadOBJModel(string fileName, bool textured)
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
    model->init(_glrenderer);

    if(!foundTexture){
        auto normal_layout = new kipod::GLRenderLayout(*layout);
        auto ebo = new kipod::ElementsBuffer(*normal_layout->ebo_);
        ebo->primitive_ = GL_POINTS;
        normal_layout->ebo_ = ebo;
        normal_layout->sha_ = &shaders_["Normals Triangles"];
        model->AddLayout({"Normals Triangles", normal_layout});
    }

}

void Scene::loadPrimitive(Primitive primitive, int numberPolygons)
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
    model->init(_glrenderer);
    {
        auto normal_layout = new kipod::GLRenderLayout(*layout);
        auto ebo = new kipod::ElementsBuffer(*normal_layout->ebo_);
        ebo->primitive_ = GL_POINTS;
        normal_layout->ebo_ = ebo;
        normal_layout->sha_ = &shaders_["Normals Triangles"];
        model->AddLayout({"Normals Triangles", normal_layout});
    }
}


void Scene::addCamera(Camera *cam, bool projective)
{  
	cameras.push_back(cam);
    camerasMode.push_back(projective);
}

void Scene::setCameraMode(int camera_id, bool perspective)
{
    camerasMode[camera_id] = perspective;
}

void Scene::setLastCameraActive()
{
    if(cameras.empty()) return;
    activeCamera = cameras.size()-1;
}

void Scene::addLight(Light *light)
{
    lights.push_back(light);
}

void Scene::AddPointSet(PointSet *point_set)
{
    point_sets_.push_back(point_set);
}

void Scene::AddShape(Shape *shape)
{
    shapes_.push_back(shape);
}

void Scene::init()
{
    if(_glrenderer) {
        _glrenderer->SetProgram();
        _glrenderer->SetProgramWithNormals();
        _glrenderer->SetProgramWithNormals_from_faces();

        _glrenderer->SetProgram(QuasiCrystal());
        _glrenderer->SetProgram(Lights());
        _glrenderer->SetProgramTex();
	}
    Camera* cam = new Camera(45, float(width_)/height_, 0.1f, 200.0);
    cam->createFrustum(); // Needed for very first Camera
    addCamera(cam);

    Light* light = new Light(LightSource::AMBIENT, vec4(0.0f), vec4(0.1, 0.1, 0.1, 1.0));
    addLight(light);
    Light* light1 = new Light(LightSource::DIFFUSE, vec4(10.0f,1.0f,0.0f,1.0f), vec4(0.2, 0.3, 0.6, 1.0));
    addLight(light1);
    Light* light2 = new Light(LightSource::SPECULAR, vec4(0.0f,1.0f,10.0f,1.0f), vec4(1.0f));
    addLight(light2);
}

void Scene::drawBoundingBox()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    boundingBox.draw(_glrenderer);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void Scene::SetupUniforms()
{
    shaders_.insert({"Textured Triangles", kipod::Shader("textures.vert.glsl", "textures.frag.glsl")});
    shaders_.insert({"Colored Triangles", kipod::Shader("lights.vert.glsl",   "lights.frag.glsl")});

    shaders_["Textured Triangles"].AttachUniform<float>("tex");
    for(auto& [name, shader]: shaders_){
        LOG_ENGINE("Attaching Uniforms to Shader {}", name);
        shader.AttachUniform<glm::mat4>("v");
        shader.AttachUniform<glm::mat4>("mv");
        shader.AttachUniform<glm::mat4>("mv_normal");
        shader.AttachUniform<glm::mat4>("projection");

        AttachMaterialToShader(shader);

        shader.AttachUniform<glm::vec4>("cameraLocation");

        for(int i = 0; i<3; ++i){
            AttachLightToShader(shader, i);
        }
    }

    shaders_.insert({"Normals Triangles", kipod::Shader("normals.vert.glsl", "normals.frag.glsl", "normals.geom.glsl")});
    auto shader = shaders_["Normals Triangles"];
    shader.AttachUniform<float>("normal_length");
    shader.AttachUniform<glm::mat4>("mv");
    shader.AttachUniform<glm::mat4>("mv_normal");
    shader.AttachUniform<glm::mat4>("projection");
}




void Scene::draw()
{
	mat4 p = cameras[activeCamera]->getProjection(camerasMode[activeCamera]);
	mat4 v = cameras[activeCamera]->getcTransform();
	mat4 camMatrix = p*v;

    //kipod::RenderManager::Bind(pointsetToTexture_mode);
    framebuffer_->Bind();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto point_set : point_sets_){
        glEnable( GL_BLEND );

        mat4 basis = point_set->GetWorldTransform();

        if(point_set->lattice_data_->qc.window == WindowType::Octagon){
            _glrenderer->useProgram(QuasiCrystal(WindowType::Octagon));
            _glrenderer->SetUniform(QuasiCrystal(WindowType::Octagon), camMatrix, basis, point_set->lattice_data_);
        }
        else if(point_set->lattice_data_->qc.window == WindowType::Box){
            _glrenderer->useProgram(QuasiCrystal());
            _glrenderer->SetUniform(QuasiCrystal(), camMatrix, basis, point_set->lattice_data_);
            _glrenderer->setUniformBlock(point_set->lattice_data_, ((QuaCry*)point_set)->window_size_);
        }
        point_set->Draw(_glrenderer);

        glEnable(GL_DEPTH_TEST);
        Shape* shape = (Shape*)(QuaCry*)point_set;
        mat4 shape_matrix = shape->GetWorldTransform();

        _glrenderer->useProgram(Shape2d());
        _glrenderer->SetUniform(Shape2d(), shape_matrix, shape);
        shape->Draw(_glrenderer);

        _glrenderer->useProgramWindow(QuasiCrystal(WindowType::Octagon));
        _glrenderer->SetUniform(QuasiCrystal(), camMatrix, basis, point_set->lattice_data_, shape);
        ((QuaCry*)point_set)->DrawWindow(_glrenderer);
        glDisable(GL_DEPTH_TEST);

        glDisable( GL_BLEND );
    }

//    for(auto shape : shapes_){
//        glEnable(GL_DEPTH_TEST);
//        _glrenderer->useProgram(Shape2d());
//        mat4 m = shape->GetWorldTransform();
//        _glrenderer->SetUniform(Shape2d(), m);
//        shape->Draw(_glrenderer);
//        glDisable(GL_DEPTH_TEST);
//    }

    kipod::RenderManager::Bind(0);

    //Draw();
	for(auto model : models){

		mat4 m = model->getmTransform();
		mat4 mvp = camMatrix * m;

        if(wireframemode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);
        if( texture_mode && model->HasLayout("Textured Triangles") ){ 
            shaders_["Textured Triangles"].Use();
           SetUniformTex(lights, cameras[activeCamera], model);
           static_cast<kipod::RenderObject*>(model)->Draw("Textured Triangles");
        }
        else if((color_mode || emissive_mode )&& model->HasLayout("Colored Triangles")  ){
            shaders_["Colored Triangles"].Use();
             SetUniform(lights, cameras[activeCamera], model);
            static_cast<kipod::RenderObject*>(model)->Draw("Colored Triangles");
        }
        glDisable(GL_DEPTH_TEST);
        if(wireframemode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if(normals_mode && model->HasLayout("Normals Triangles") ){
            shaders_["Normals Triangles"].Use();
            SetUniformNormal(model, cameras[activeCamera]);
            static_cast<kipod::RenderObject*>(model)->Draw("Normals Triangles");
        }
		if(box_mode){
                _glrenderer->useProgram(1);
				mvp = camMatrix * model->getmTransformBBox();
                _glrenderer->SetUniformMVP(mvp);
                drawBoundingBox();
		}
	}
	if(camera_mode){
        _glrenderer->useProgram(1);
		for(auto cam : cameras){
			mat4 mvp = camMatrix * Translate((cam->getEye()));
            _glrenderer->SetUniformMVP(mvp);
			cam->draw();
		}
	}
	if(camera_frustum_mode){
        _glrenderer->useProgram(1);
		for(auto cam : cameras){
			mat4 mvp = camMatrix * Inverse(cam->getcTransform()) * Inverse(cam->getProjection(true));
            _glrenderer->SetUniformMVP(mvp);
			cam->drawFrustum();
		}
	}
}


void Scene::drawSoft()
{
    mat4 p = cameras[activeCamera]->getProjection(camerasMode[activeCamera]);
    mat4 v = cameras[activeCamera]->getcTransform();
    _softrenderer->SetCameraTransform(v);
    _softrenderer->SetProjection(p);
    for(auto model : models){
        mat4 m = model->getmTransform();
        mat3 n = mat3(vec3(&m[0][0]),vec3(&m[1][0]),vec3(&m[2][0]));
        n = transpose(Inverse(n));
        _softrenderer->SetObjectMatrices(m,n);

        if(color_mode || emissive_mode)
            model->drawWithLight(_softrenderer,lights, color_mode, emissive_mode);
        else
            model->draw(_softrenderer, wireframemode, clipping_mode, normals_mode);

        if(box_mode){
             mat4 m = model->getmTransformBBox();
             _softrenderer->SetObjectMatrices(m, mat3(1.0));
             boundingBox.draw(_softrenderer, true,false);
        }
    }
}

void Scene::drawDemo()
{
    _softrenderer->SetDemoBuffer();
}


void Scene::clearBuffer()
{
    _softrenderer->ClearBuffer();
}

void Scene::swapBuffers()
{
    _softrenderer->SwapBuffers();
}

Camera* Scene::getActiveCamera(){ return cameras[activeCamera];}

MeshModel* Scene::getActiveModel(){ return models[activeModel]; }

void Scene::moveCamera(int camera_id, const vec3& eye, const vec3& at, const vec3& up ){
	if(numberOfCameras() <= camera_id) return;
	Camera* cam = cameras[camera_id];
	cam->updateLookAt( eye, at, up );
}

void Scene::moveEyeOfCamera(int camera_id, const vec3& eye){
	if(numberOfCameras() <= camera_id) return;
	Camera* cam = cameras[camera_id];
	cam->moveEye(eye);
}

void Scene::perspectiveCamera(int camera_id, const float& fovy, const float& aspect, const float& near, const float& far){
	if(numberOfCameras() <= camera_id) return;
	Camera* cam =cameras[camera_id];
	cam->Perspective(fovy, aspect, near, far);
    needs_update = true;
}


void Scene::moveModel(int model_id, const vec3& translate){
	if(numberOfModels() <= model_id) return;
	MeshModel* model = models[model_id];
	model->move(translate);
    needs_update = true;
}

void Scene::lookAtModel(int camera_id, int model_id){
	if(numberOfModels() <= model_id) return;
	if(numberOfCameras() <= camera_id) return;
	Camera* cam =cameras[camera_id];
	MeshModel* model = models[model_id];
	cam->updateAt(model->getCenter());
}


void Scene::processEvent(Event& event){
    if(event.getType() == EventType::TranslateCamera){
        needs_update = true;
        if(event.getData().direction == MoveDirection::RIGHT)   {
            moveEyeOfCamera(activeCamera, vec3(1,0,0));
            return;
        }
        if(event.getData().direction == MoveDirection::LEFT){
            moveEyeOfCamera(activeCamera, vec3(-1,0,0));
        return;
        }
        if(event.getData().direction == MoveDirection::UP) {
            moveEyeOfCamera(activeCamera, vec3(0,1,0));
        return;
        }
        if(event.getData().direction == MoveDirection::DOWN) {
            moveEyeOfCamera(activeCamera, vec3(0,-1,0));
        return;
        }
        if(event.getData().direction == MoveDirection::FORWARD)  {
            moveEyeOfCamera(activeCamera, vec3(0,0,1));
        return;
        }
        if(event.getData().direction == MoveDirection::BACKWARD){
            moveEyeOfCamera(activeCamera, vec3(0,0,-1));
        return;
        }
    }
    if(event.getType() == EventType::TranslateModel){
        needs_update = true;
        if(event.getData().direction == MoveDirection::RIGHT)  { moveModel(activeModel, vec3(1,0,0)); return; }
        if(event.getData().direction == MoveDirection::LEFT) {  moveModel(activeModel, vec3(-1,0,0)); return; }
        if(event.getData().direction == MoveDirection::UP)  { moveModel(activeModel, vec3(0,1,0)); return; }
        if(event.getData().direction == MoveDirection::DOWN) {  moveModel(activeModel, vec3(0,-1,0)); return; }
        if(event.getData().direction == MoveDirection::FORWARD)  { moveModel(activeModel, vec3(0,0,1)); return; }
        if(event.getData().direction == MoveDirection::BACKWARD)  { moveModel(activeModel, vec3(0,0,-1)); return; }
    }

    if(event.getType() == EventType::RotateModel){
        MeshModel* model = getActiveModel();
        needs_update = true;
        if(event.getData().rotateDiraction == RotateDirection::ROLL)  { model->moveLocal(RotateX(event.getData().amount)); return; }
        if(event.getData().rotateDiraction == RotateDirection::YAW) { model->moveLocal(RotateY(event.getData().amount)); return; }
        if(event.getData().rotateDiraction == RotateDirection::PITCH)  { model->moveLocal(RotateZ(event.getData().amount)); return; }
    }
    if(event.getType() == EventType::SceneUpdate){
        needs_update = true;
        return;
    }
    if(event.getType() == EventType::WireframeMode){
        LOG_DEBUG("Switched Wireframe Mode");
        wireframemode = !wireframemode;
        color_mode = !wireframemode;
        emissive_mode = !wireframemode;
        return;
    }
    if(event.getType() == EventType::EmissiveMode){
        emissive_mode = !emissive_mode;
        return;
    }
    if(event.getType() == EventType::LightMode){
        color_mode = !color_mode;
        return;
    }
    if(event.getType() == EventType::BoxMode){
        LOG_DEBUG("Switched Box Mode");
        box_mode = !box_mode;
        return;
    }
    if(event.getType() == EventType::NormalMode){
        LOG_DEBUG("Switched Normal Mode");
        normals_mode = !normals_mode;
        return;
    }
    if(event.getType() == EventType::CameraMode){
        LOG_DEBUG("Camera Box Mode");
        camera_mode = !camera_mode;
        return;
    }
    if(event.getType() == EventType::ClippingMode){
        clipping_mode = !clipping_mode;
        return;
    }
    if(event.getType() == EventType::TextureMode){
        texture_mode = !texture_mode;
        return;
    }
}


