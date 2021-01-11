#pragma once

#include <vector>
#include "utils/vec.h"
#include "utils/mat.h"
#include "softrenderer.h"
#include "glrenderer.h"
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "eventmanager.h"


using namespace std;

class Model {
protected:
    shared_ptr<ModelData> modelData= nullptr;
	virtual ~Model() {}
	void virtual draw()=0;
};

struct BoundingBoxData{
    vec3 _size;
    vec3 _center;
    mat4 _transform;

    BoundingBoxData(){}
    BoundingBoxData(const vector<vec3> &vertices);
};


class MeshModel : public Model, public Listener
{
protected :
	MeshModel() {}
	//vec3 *vertex_positions;
	//add more attributes

	mat4 _local_transform;
	mat4 _world_transform;
	mat3 _normal_transform;

    BoundingBoxData _boundingBoxData;
protected:
    shared_ptr<ModelData> modelDataWired = nullptr;

public:
    vector<vec3> vertices_vector;
    vector<vec3> normals_vector;
    vector<vec2> texture_vector;

    vector<unsigned int> indices_vector;
    vector<unsigned int> nindices_vector;
    vector<unsigned int> tindices_vector;


    vector<MaterialStruct> colors_vector;
    vector<unsigned int> cindices_vector;


    MeshModel(string fileName);
	~MeshModel(void);
	void loadFile(string fileName);
    void calculateNormals();
    void reduceVertices();


    void init(GLRenderer *glrenderer, bool colored = true);
    void draw() override{}

    void draw(GLRenderer *glrenderer);
    void drawNormals(GLRenderer *glrenderer);
    void draw(SoftRenderer *softrenderer, bool wireframemode, bool clippingMode, bool normals=false);
    void drawWithLight(SoftRenderer *softrenderer, const std::vector<Light*> &lights, bool lightMode=true, bool emissiveMode=false);

    void drawColored(GLRenderer *glrenderer);

	void createBBox();
	vec3 getCenter();
	void centerModel();
	void move(const vec3& translate);

	void moveLocal(const mat4& transform);
	void moveWorld(const mat4& transform);

    void setLocal(const mat4& transform);

    mat4 getmTransform();
    mat4 getmTransformBBox();

    float normal_length=1;

    void processEvent(Event& event) override;
    void setUniformMaterial(MaterialStruct &material);
    void setUniformMaterial();
};
