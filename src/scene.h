#pragma once

#include "utils/vec.h"
#include "GL/glew.h"
#include <vector>
#include <string>
#include "glrenderer.h"
#include "softrenderer.h"
#include "meshmodel.h"
#include "primmeshmodel.h"
#include "camera.h"
#include "light.h"
#include "eventmanager.h"


#include "pointset.h"
#include "shapes.h"


#include "render/render_manager.h"
#include "render/render_object.h"
#include "render/render_scene.h"
#include "render/render_material.h"

#include "engine/engine_gui.h"
#include "engine/engine_events.h"
#include "engine/engine_input.h"
#include "engine/engine_controls.h"


class QuaCry;

using namespace std;

class Scene : public Listener, public kipod::Listener,
        public kipod::Controls, public kipod::RenderScene{

    std::unordered_map<std::string, kipod::Shader> shaders_;

    friend class GUI;
    friend class kipod::Gui;
    friend class QuaCry;


    vector<MeshModel*> models;
    vector<Light*> lights;
    vector<Camera*> cameras;
    vector<Shape*> shapes_;
    vector<PointSet*> point_sets_;

    GLRenderer *_glrenderer;
    SoftRenderer *_softrenderer;

    vector<bool> camerasMode;

    PrimMeshModel boundingBox;

    void drawBoundingBox();


protected:


public:
    Scene(GLRenderer *renderer=nullptr, SoftRenderer *softrenderer=nullptr,
          unsigned int width=800, unsigned int height=600)
        : RenderScene(width, height), _glrenderer(renderer), _softrenderer(softrenderer),
          boundingBox(Cube)
    {

    }

    void loadOBJModel(string fileName, bool textures = false);
    void loadPrimitive(Primitive primitive, int numberPolygons=0);

    virtual void Setup() override;
    virtual void Draw() override {} ;

    void SetupUniforms();

	Camera* getActiveCamera();
	MeshModel* getActiveModel();
    void addCamera(Camera *cam, bool projective=true);
	void moveCamera(int camera_id, const vec3& eye, const vec3& at, const vec3& up );
	void moveEyeOfCamera(int camera_id, const vec3& eye);
	void perspectiveCamera(int camera_id, const float& fovy, const float& aspect, const float& near, const float& far);
	void moveModel(int model_id, const vec3& translate);
	void lookAtModel(int camera_id, int model_id);

    void setCameraMode(int camera_id, bool projective);

    void setLastCameraActive();

    void addLight(Light *light);
    bool hasLight() { return !lights.empty(); }

    void AddPointSet(PointSet *point_set);
    bool HasPointSet(){ return !point_sets_.empty(); }
    void AddShape(Shape *shape);

	void init();
	void draw();
	void drawDemo();

	void clearBuffer();
	void swapBuffers();

    void drawSoft();

	int numberOfCameras(){ return cameras.size();}
	int numberOfModels(){ return models.size();}

	void setActiveCamera(int id){
		if(id<numberOfCameras()) activeCamera = id;
	}
	void setActiveModel(int id){
		if(id<numberOfModels()) activeModel = id;
	}
    int activeModel=-1;
	int activeLight;
	int activeCamera=0;

	bool normals_mode=false;
	bool camera_mode = false;
    bool camera_frustum_mode = false;
    bool color_mode = true;
    bool texture_mode = true;

    bool box_mode = false;
    bool wireframemode = false;
    bool emissive_mode = false;
    bool clipping_mode = true;

    int pointsetToTexture_mode = 0;

    bool cg_active = false;
    bool needs_update = false;

    void processEvent(Event& event) override;

    void BindMaterialUniforms(kipod::Shader& shader, const kipod::RenderMaterial &material);
    void BindLightUniforms(kipod::Shader& shader, vector<Light *> &lights);
    void BindMatrixUniforms(kipod::Shader& shader, const kipod::RenderObject &model, const Camera &camera);
    void BindMatrixUniformsForMesh(kipod::Shader& shader, const MeshModel &model, const Camera &camera);
    void BindTextureUniforms(kipod::Shader& shader, const kipod::Texture *texture);
    void BindNormalUniforms(kipod::Shader& shader, const float length);

    void SetUniform(vector<Light *> &lights, Camera *camera, MeshModel *model);
    void SetUniformNormal(MeshModel *model, Camera *camera);
    void SetUniformTex(vector<Light *> &lights, Camera *camera, MeshModel *model);
    void SetUniformBox(MeshModel *model);
    void SetShapeUniform(Shape *shape);

    void SetUniformInternal(Camera *camera, QuaCry *quacry);
    void SetUniformPhysical(Camera *camera, QuaCry *quacry);
    void SetUniformPhysicalBox(Camera *camera, QuaCry *quacry);

    void SetupShaders();
    void SetupShaderBasic();
    void SetupShaderNormals();
    void SetupShaderColoredTriangles();
    void SetupShaderTexturedTriangles();
    void SetupShaderShape();

    void SetupShaderPointSet();
    void SetupShaderQuasi();
    void SetupBlockUniform(QuaCry *quacry);

    virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;
    LISTENER_SIGNUP(kipod::EventCategoryKeyboard)
    virtual void Receive(std::shared_ptr<kipod::Event> event) override{
        Process<kipod::KeyPressedEvent>(event, BIND_EVENT_FN(Scene::ProcessKeys));
    }

    void SetupOptions();

    virtual void DrawGui() override;
};
