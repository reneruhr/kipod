#include "../include/core.h"
#include "../include/scene.h"
#include "../include/meshmodel.h"

#include "../include/quacry.h"

using namespace std;

void Scene::loadOBJModel(string fileName, MaterialStruct material)
{
	MeshModel *model = new MeshModel(fileName);
    model->setUniformMaterial(material);
	models.push_back(model);
}

void Scene::loadPrimitive(Primitive primitive, MaterialStruct material, int numberPolygons){
    PrimMeshModel *model = new PrimMeshModel(primitive, numberPolygons);
    model->setUniformMaterial(material);
	models.push_back(model);
}

void Scene::initLastModel(){
    //models.back()->init();
    models.back()->init(_glrenderer);
}

void Scene::addCamera(Camera *cam, bool projective)
{  
	cameras.push_back(cam);
    camerasMode.push_back(projective);
}

void Scene::setCameraMode(int camera_id, bool perspective){
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

void Scene::init(){
    if(_glrenderer) {
        _glrenderer->SetProgram();
        _glrenderer->SetProgramWithNormals();
        _glrenderer->SetProgramWithNormals_from_faces();

        _glrenderer->SetProgram(QuasiCrystal());
        _glrenderer->SetProgram(Lights());
	}
    Camera* cam = new Camera(45, float(_width)/_height, 0.1f, 200.0);
    cam->createFrustum(); // Needed for very first Camera
    addCamera(cam);

    Light* light = new Light(LightSource::AMBIENT, vec4(0.0f), vec4(0.1, 0.1, 0.1, 1.0));
    addLight(light);
    Light* light1 = new Light(LightSource::DIFFUSE, vec4(10.0f,1.0f,0.0f,1.0f), vec4(0.2, 0.3, 0.6, 1.0));
    addLight(light1);
    Light* light2 = new Light(LightSource::SPECULAR, vec4(0.0f,1.0f,10.0f,1.0f), vec4(1.0f));
    addLight(light2);
}

void Scene::drawBoundingBox(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    boundingBox.draw(_glrenderer);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Scene::draw()
{
	mat4 p = cameras[activeCamera]->getProjection(camerasMode[activeCamera]);
	mat4 v = cameras[activeCamera]->getcTransform();
	mat4 camMatrix = p*v;

    for(auto point_set : point_sets_){
        if(point_set->lattice_data_->qc.window == WindowType::Octagon){
            _glrenderer->useProgram(QuasiCrystal(WindowType::Octagon));
            mat4 basis = point_set->GetWorldTransform();
            _glrenderer->SetUniform(QuasiCrystal(WindowType::Octagon), camMatrix, basis);
            _glrenderer->setUniformBlock(point_set->lattice_data_, ((QuaCry*)point_set)->window_vertices_);
        }
        else if(point_set->lattice_data_->qc.window == WindowType::Box){
            _glrenderer->useProgram(QuasiCrystal());
            mat4 basis = point_set->GetWorldTransform();
            _glrenderer->SetUniform(QuasiCrystal(), camMatrix, basis);
            _glrenderer->setUniformBlock(point_set->lattice_data_, ((QuaCry*)point_set)->window_size_);
        }
        point_set->Draw(_glrenderer);
    }

    for(auto shape : shapes_){
        _glrenderer->useProgram(Shape2d());
        mat4 m = shape->GetWorldTransform();
        _glrenderer->SetUniform(Shape2d(), m);
        shape->Draw(_glrenderer);
    }

	for(auto model : models){

		mat4 m = model->getmTransform();
		mat4 mvp = camMatrix * m;

        if(color_mode || emissive_mode){
            _glrenderer->useProgram(Lights());
            _glrenderer->SetUniform(m, v, p, lights, model->colors_vector[0], cameras[activeCamera]);
            model->drawColored(_glrenderer);
        }
        else if (wireframemode)   {
            _glrenderer->useProgram(1);
            _glrenderer->SetUniformMVP(mvp);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            model->draw(_glrenderer);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

		if(normals_mode){
            _glrenderer->useProgram(2);
            _glrenderer->SetUniformMVP_Normal(p, v, m);
            _glrenderer->SetUniformNormalLength(model->normal_length, 2);
            model->drawNormals(_glrenderer);
            _glrenderer->useProgram(1);
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
}


