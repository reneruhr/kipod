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

#include "render_object.h"

using namespace std;

class Model {
public:
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



class MeshModel : public Model, public Listener, public kipod::RenderObject
{
protected :
	MeshModel() {}
	//vec3 *vertex_positions;
	//add more attributes

	mat4 _local_transform;
	mat4 _world_transform;
	mat3 _normal_transform;

    BoundingBoxData _boundingBoxData;

public:
    MeshModel(string fileName, bool textured = false);
    ~MeshModel(void);

    shared_ptr<ModelData> modelDataWired = nullptr;
    shared_ptr<ModelData> modelTexturedData = nullptr;

    vector<vec3> vertices_vector;
    vector<vec3> normals_vector;
    vector<vec2> texture_vector;

    vector<unsigned int> indices_vector;
    vector<unsigned int> nindices_vector;
    vector<unsigned int> tindices_vector;

    vector<GLTriangle> triangles_;
    vector<unsigned int> triangles_indices_;

    kipod::RenderMaterial* material = nullptr;

    void CreateTriangleVector()
    {
        for(int i=0, n=indices_vector.size(); i<n; i+=3){
                     triangles_.emplace_back(
                     GLTriangle(GLVertex(
                                  vertices_vector[indices_vector[i]],
                                  normals_vector[nindices_vector[i]],
                                  texture_vector[tindices_vector[i]]),
                                GLVertex(
                                    vertices_vector[indices_vector[i+1]],
                                    normals_vector[nindices_vector[i+1]],
                                    texture_vector[tindices_vector[i+1]]),
                                GLVertex(
                                    vertices_vector[indices_vector[i+2]],
                                    normals_vector[nindices_vector[i+2]],
                                    texture_vector[tindices_vector[i+2]])
                                ));
        }
    }


    vector<MaterialStruct> colors_vector;
    vector<unsigned int> cindices_vector;


//    virtual void Setup() override;


	void loadFile(string fileName);
    void calculateNormals();
    void reduceVertices();


    void Init(bool textured, bool normals=true);

    void Draw(GLRenderer *glrenderer);

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

    mat4 getmTransform() const;
    mat4 getmTransformBBox() const;

    float normal_length=1;

    void processEvent(Event& event) override;
    void setUniformMaterial(MaterialStruct &material);
    void setUniformMaterial();
    void init(GLRenderer* glrenderer);
};
