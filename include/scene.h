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

#include "pointset.h"

class QuaCry;

using namespace std;

class Scene : public Listener{
    friend class GUI;
    friend class QuaCry;

    vector<MeshModel*> models;
    vector<Light*> lights;
    vector<Camera*> cameras;
    GLRenderer *_glrenderer;
    SoftRenderer *_softrenderer;

    vector<bool> camerasMode;
    unsigned int _width,_height;

    PrimMeshModel boundingBox;

    void drawBoundingBox();

protected:
    vector<PointSet*> pointsets;



public:
    Scene(GLRenderer *renderer=nullptr, SoftRenderer *softrenderer=nullptr,
          unsigned int width=800, unsigned int height=600)
        :_glrenderer(renderer), _softrenderer(softrenderer),
          _width(width), _height(height), boundingBox(Cube)
    {
        boundingBox.init(_glrenderer);
    }

    void loadOBJModel(string fileName, MaterialStruct material);
    void loadPrimitive(Primitive primitive, MaterialStruct material, int numberPolygons=0);
	
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


	void initLastModel();
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

    bool box_mode = false;
    bool wireframemode = false;
    bool emissive_mode = false;
    bool clipping_mode = true;

    bool cg_active = false;
    bool needs_update = false;

    void processEvent(Event& event) override;
};
