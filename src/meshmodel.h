#pragma once

#include "core.h"
#include "softrenderer.h"
#include "render/render_object.h"
#include "engine/engine_console.h"


struct BoundingBoxData{
    glm::vec3 size_;
    glm::vec3 center_;
    glm::mat4 transform_;

    BoundingBoxData() = default;
    BoundingBoxData(const vector<vec3> &vertices);
    BoundingBoxData(BoundingBoxData&&) = default;
    BoundingBoxData& operator=(BoundingBoxData&&) = default;
};


class MeshModel : public kipod::RenderObject
{
protected :
	mat4 _local_transform;
	mat4 _world_transform;
	mat3 _normal_transform;

    std::unique_ptr<BoundingBoxData> boundingBoxData_;
public:
    MeshModel() = default;
    MeshModel(string fileName, bool textured = false);
    virtual ~MeshModel(void);
    MeshModel(MeshModel&&) = default;
    MeshModel& operator=(MeshModel&&) = default;

    vector<vec3> vertices_vector;
    vector<vec3> normals_vector;
    vector<vec2> texture_vector;

    vector<unsigned int> indices_vector;
    vector<unsigned int> nindices_vector;
    vector<unsigned int> tindices_vector;

    vector<GLTriangle> triangles_;
    vector<unsigned int> triangles_indices_;

    kipod::RenderMaterial* material = nullptr;

    float normal_length=1;

    vector<MaterialStruct> colors_vector;
    vector<unsigned int> cindices_vector;

    void LoadFile(string fileName);
    void CalculateNormals();
    void ReduceVertices();
    bool Valid();
    void CreateTriangleVector();


    void Init(bool textured, bool normals=true);

    void setUniformMaterial(MaterialStruct &material);
    void setUniformMaterial();


    void CreateBoundingBox();
    void CenterModel();
    glm::mat4 TansformBoundingBox() ;


    void draw(SoftRenderer *softrenderer, bool wireframemode, bool clippingMode, bool normals=false);
    void drawWithLight(SoftRenderer *softrenderer, const std::vector<Light*> &lights, bool lightMode=true, bool emissiveMode=false);

    vec3 getCenter();
	void move(const vec3& translate);
	void moveLocal(const mat4& transform);
	void moveWorld(const mat4& transform);
    void setLocal(const mat4& transform);
    mat4 getmTransform() const;


};
