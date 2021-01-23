#pragma once

#include "utils/vec.h"
#include "GL/glew.h"
#include <vector>
#include <string>
#include "../include/glrenderer.h"
#include "../include/softrenderer.h"
#include "../include/meshmodel.h"
#include "../include/primmeshmodel.h"
#include "../include/camera.h"
#include "../include/light.h"
#include "eventmanager.h"

#include "render_manager.h"

#include "pointset.h"
#include "shapes.h"

#include "render_object.h"
#include "render_scene.h"
#include "render_material.h"

class QuaCry;

using namespace std;

class Scene : public Listener, public kipod::RenderScene{

    std::unordered_map<std::string, kipod::Shader> shaders_;

    friend class GUI;
    friend class QuaCry;



    vector<MeshModel*> models;
    vector<Light*> lights;
    vector<Camera*> cameras;
    vector<Shape*> shapes_;
    vector<PointSet*> point_sets_;

    GLRenderer *_glrenderer;
    SoftRenderer *_softrenderer;

    vector<bool> camerasMode;
    unsigned int _width,_height;

    PrimMeshModel boundingBox;

    void drawBoundingBox();

protected:


public:
    Scene(GLRenderer *renderer=nullptr, SoftRenderer *softrenderer=nullptr,
          unsigned int width=800, unsigned int height=600)
        :_glrenderer(renderer), _softrenderer(softrenderer),
          _width(width), _height(height), boundingBox(Cube)
    {
        boundingBox.init(_glrenderer);
    }


    void loadOBJModel(string fileName, bool textures = false);
    void loadPrimitive(Primitive primitive, int numberPolygons=0);


    virtual void Setup() override;
    virtual void Draw() override;
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

    void initLastModel(bool with_texture = false);
	void init();
	void draw();
	void drawDemo();
//	void drawLine(int p[2], int q[2]);
//    void drawLineDDA(int p[2], int q[2]) { _softrenderer->drawLineDDA(p,q);}
//    void drawLineMidpoint(int p[2], int q[2]) { _softrenderer->drawLineMidpoint(p,q);}
//    void drawLineBresenheim(int p[2], int q[2]) { _softrenderer->drawLineBresenheim(p,q);}

//	void drawTestTriangle();
//    void drawTestTriangle2();
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
	int activeModel=0;
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

    void BindMaterialUniforms(const kipod::RenderMaterial &material);
    void BindLightUniforms(vector<Light *> &lights);
    void BindMatrixUniforms(const kipod::RenderObject &model, const Camera &camera);
    void BindMatrixUniformsForMesh(const MeshModel &model, const Camera &camera);
};
