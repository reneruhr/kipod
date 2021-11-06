#pragma once
#include "../../core.h"
#include "../../render/render_object.h"
#include "../../render/opengl/opengl_layout.h"
#include "../../engine/engine_console.h"


#include "meshmodel_boundingbox.h"

namespace kipod::MeshModels{
using Vec3 = glm::vec3;
struct Triangle3f { Vec3 a,b,c; };
using Vec2 = glm::vec2;
class MeshModelScene;
class MeshModel : public kipod::RenderObject
{
    friend class SoftRendererScene;
protected :
    std::shared_ptr< std::vector<Vec3> > vertices_vector;
    std::shared_ptr< std::vector<Vec3> > normals_vector;
    std::shared_ptr< std::vector<Vec2> > texture_vector;

    std::shared_ptr< std::vector<unsigned int> > indices_vector;
    std::shared_ptr< std::vector<unsigned int> > nindices_vector;
    std::shared_ptr< std::vector<unsigned int> > tindices_vector;

    std::vector<kipod::GLTriangle> triangles_;

    std::shared_ptr<BoundingBoxData> boundingBoxData_;
public:
    MeshModel() = default;
    MeshModel(std::filesystem::path path, bool textured = false);
    MeshModel(const std::vector<Vec3>&, const std::vector<unsigned int>&);
    virtual ~MeshModel(void);
    MeshModel(const MeshModel&) = default;
    MeshModel& operator=(const MeshModel&) = default;
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

    auto Vertices() -> std::vector<Vec3>*;
    auto Triangle(int n) const -> Triangle3f;
    int NumberOfTriangles() const { return indices_vector->size()/3; }
    void TriangleFlip(int n) {  std::swap( (*indices_vector)[3*n], (*indices_vector)[3*n+1] ); }
};

float VolumeTetrahedron(const Triangle3f& T, const glm::vec3 &p);
bool TestIfCCWOriented(const MeshModel& model);
void FixCCWOriented(MeshModel& model);
}


template<>
struct fmt::formatter<kipod::MeshModels::Triangle3f> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const kipod::MeshModels::Triangle3f& T, FormatContext& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(),
                         "{} {} {}",
                         glm::to_string(T.a), glm::to_string(T.b), glm::to_string(T.c) );
    }
};
