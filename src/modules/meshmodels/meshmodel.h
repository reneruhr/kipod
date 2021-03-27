#pragma once
#include "../../core.h"
#include "../../render/render_object.h"
#include "../../render/opengl/opengl_layout.h"
#include "../../engine/engine_console.h"


#include "meshmodel_boundingbox.h"

namespace kipod::MeshModels{
class MeshModelScene;
class MeshModel : public kipod::RenderObject
{
    friend class SoftRendererScene;
protected :
    std::shared_ptr< std::vector<vec3> > vertices_vector;
    std::shared_ptr< std::vector<vec3> > normals_vector;
    std::shared_ptr< std::vector<vec2> > texture_vector;

    std::shared_ptr< std::vector<unsigned int> > indices_vector;
    std::shared_ptr< std::vector<unsigned int> > nindices_vector;
    std::shared_ptr< std::vector<unsigned int> > tindices_vector;

    std::vector<kipod::GLTriangle> triangles_;

    std::unique_ptr<BoundingBoxData> boundingBoxData_;
public:
    MeshModel() = default;
    MeshModel(std::filesystem::path path, bool textured = false);
    virtual ~MeshModel(void);
    MeshModel(MeshModel&&) = default;
    MeshModel& operator=(MeshModel&&) = default;

    void Init(bool textured, bool normals=true, bool basic=false);

    bool Valid();
    float normal_length=1;

    void SetUniformMaterial();

    void CreateBoundingBox();
    void CenterModel();
    glm::mat4 TansformBoundingBox();
    glm::vec3 Center();

//    void draw(SoftRenderer *softrenderer, bool wireframemode, bool clippingMode, bool normals=false);
//    void drawWithLight(SoftRenderer *softrenderer, const std::vector<Light*> &lights, bool lightMode=true, bool emissiveMode=false);
};
}
