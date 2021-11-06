#include "../../core.h"
#include "meshmodel.h"
#include "../../render/render_texture.h"
#include "meshmodel_parsing.h"

namespace kipod::MeshModels{

MeshModel::MeshModel(const std::vector<Vec3>& vertices, const std::vector<unsigned int>& indices)
{
    name_ = "New Model"; 
    vertices_vector = std::make_shared<std::vector<Vec3>>(vertices);
    normals_vector = std::make_shared<std::vector<Vec3>>();
    texture_vector = std::make_shared<std::vector<Vec2>>();
    indices_vector = std::make_shared<std::vector<unsigned int>>(indices);
    nindices_vector = std::make_shared<std::vector<unsigned int>>();
    tindices_vector = std::make_shared<std::vector<unsigned int>>();
    CalculateNormals(vertices_vector, indices_vector,
                                             normals_vector, nindices_vector);
    CreateBoundingBox();
    CenterModel();
}

MeshModel::MeshModel(std::filesystem::path path, bool textured)
{
    name_ = path.stem().string();
    vertices_vector = std::make_shared<std::vector<Vec3>>();
    normals_vector = std::make_shared<std::vector<Vec3>>();
    texture_vector = std::make_shared<std::vector<Vec2>>();
    indices_vector = std::make_shared<std::vector<unsigned int>>();
    nindices_vector = std::make_shared<std::vector<unsigned int>>();
    tindices_vector = std::make_shared<std::vector<unsigned int>>();
    LoadFile(path,
                    vertices_vector, indices_vector,
                    normals_vector, nindices_vector,
                    texture_vector, tindices_vector);
    if(normals_vector->empty()) CalculateNormals(vertices_vector, indices_vector,
                                             normals_vector, nindices_vector);
    if(Valid()==false) {
        LOG_CONSOLE("[error] Could not load Object Model");
        LOG_ENGINE("Could not load Object Model");
        return;
    }
    CreateBoundingBox();
    CenterModel();


    if(textured){
        tex_ = std::make_shared<kipod::Texture>();

        auto path_png = path.replace_extension(".png");
        std::cout << path_png;
        if(std::filesystem::exists(path_png)){
            tex_->LoadTexture(path_png);
            return;
        }
        auto path_svg = path.replace_extension(".svg");
        if(std::filesystem::exists(path_svg)){
            tex_->LoadTexture(path_svg);
            return;
         }
        auto path_jpg = path.replace_extension(".jpg");
        if(std::filesystem::exists(path_jpg)){
            tex_->LoadTexture(path_jpg);
            return;
         }
        LOG_INFO("Tried but did not succeed to load texture.");
        tex_ = nullptr;
    }
}

MeshModel::~MeshModel(void)
{
}

bool MeshModel::Valid()
{
    return !vertices_vector->empty();
}

void MeshModel::Init(bool textured, bool normals, bool basic)
{
    if(textured){
        kipod::CreateTriangleVector(triangles_,
                                    *vertices_vector, *indices_vector,
                                    *normals_vector, *nindices_vector,
                                    *texture_vector, *tindices_vector);
        auto layout = Layout("Textured Triangles");
        static_cast<kipod::GLRenderLayout*>(layout)->SetupGLTriangles(&triangles_);
    }else if(normals){
        auto layout = static_cast<kipod::GLRenderLayout*>(Layout("Colored Triangles"));
        layout->SetupColoredTriangles(vertices_vector.get(), indices_vector.get(),
                                               normals_vector.get(), nindices_vector.get());
    }else if(HasLayout("Colored Triangles")){
        auto layout = static_cast<kipod::GLRenderLayout*>(Layout("Colored Triangles"));
        layout->SetupColoredTriangles(vertices_vector.get(),indices_vector.get());
    }

    if(basic){
        auto layout = static_cast<kipod::GLRenderLayout*>(Layout("Basic"));
        layout->SetupColoredTriangles(vertices_vector.get(),indices_vector.get());
    }
}

//void MeshModel::draw(SoftRenderer *softrenderer, bool wireframemode, bool clippingMode, bool normals)
//{
//    if(normals)
//        softrenderer->DrawTriangles(&vertices_vector, &indices_vector,
//                                wireframemode, clippingMode,
//                                &normals_vector, &nindices_vector);
//    else
//        softrenderer->DrawTriangles(&vertices_vector, &indices_vector, wireframemode);
//}

//void MeshModel::drawWithLight(SoftRenderer *softrenderer, const std::vector<Light*> &lights, bool lightMode, bool emissiveMode){
//        softrenderer->DrawColoredTriangles(&vertices_vector, &indices_vector,
//                                    &normals_vector, &nindices_vector,
//                                    &colors_vector, &cindices_vector,
//                                    lights,lightMode, emissiveMode);
//}

void MeshModel::CreateBoundingBox()
{
    boundingBoxData_ = std::make_unique<BoundingBoxData>(*vertices_vector);
}

glm::mat4 MeshModel::TansformBoundingBox()
{
    return Transform() * boundingBoxData_->transform_;
}

glm::vec3 MeshModel::Center()
{
    glm::mat4 m = TransformWorld()*TransformLocal();
    glm::vec3 center = glm::vec3(m[3][0],m[3][1],m[3][2]);
	return center;
}

void MeshModel::CenterModel()
{
    local_->Translate(-boundingBoxData_->center_);
}

void MeshModel::SetUniformMaterial()
{
    mat_ =  std::make_shared<kipod::RenderMaterial>();
}

auto MeshModel::Vertices() -> std::vector<Vec3>*
{
    return vertices_vector.get();
}
auto MeshModel::Triangle(int n) const -> Triangle3f
{
    auto m = 3*n;
    assert(m<indices_vector->size());
    auto v = [this](int i){ return  (*vertices_vector)[(*indices_vector)[i]]; };
    return { v(m), v(m+1), v(m+2) };
}

// Volume x6
float VolumeTetrahedron(const Triangle3f& T, const glm::vec3 &p)
{
    glm::mat4 in( Vec4(T.a,1), Vec4(T.b,1), Vec4(T.c,1), Vec4(p, 1));
    LOG_DEBUG("Apply Determinant to {}", in);
    return determinant(in);
}

//Assumes 0 is inside.
bool TestIfCCWOriented(const MeshModel &model)
{
    auto Inside = [&model](int n){
        return VolumeTetrahedron(model.Triangle(n), Vec3(0)) > 0;
    };
    for(int i = 0, e = model.NumberOfTriangles(); i<e; ++i)
        if(!Inside(i)) return false;
    return true;
}

void FixCCWOriented(MeshModel& model)
{
    LOG_DEBUG("Call to Fix Orientation. Running through {} triangles", model.NumberOfTriangles());
    auto Inside = [&model](int n) {
        return VolumeTetrahedron(model.Triangle(n), Vec3(0)) > 0;
    };
    for(int i = 0, e = model.NumberOfTriangles(); i<e; ++i) {
        LOG_DEBUG("Test {}th triangle", i);
        if (!Inside(i)) {
            LOG_DEBUG("Found CW triangle. Flip now.");
            model.TriangleFlip(i);
            LOG_DEBUG("Orientation is fixed: {}", Inside(i));
        }
    }
}

}

