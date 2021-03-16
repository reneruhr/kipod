#include "../../core.h"


#include "meshmodel.h"
#include "../../render/render_texture.h"
#include "meshmodel_parsing.h"

namespace kipod::MeshModels{

MeshModel::MeshModel(std::filesystem::path path, bool textured)
{
    name_ = path.stem().string();
    LoadFile(path,
                    vertices_vector, indices_vector,
                    normals_vector, nindices_vector,
                    texture_vector, tindices_vector);
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
    return !vertices_vector.empty();
}



void MeshModel::Init(bool textured, bool normals)
{
    if(textured){
        kipod::CreateTriangleVector(triangles_,
                                    vertices_vector, indices_vector,
                                    normals_vector, nindices_vector,
                                    texture_vector, tindices_vector);
        auto layout = Layout("Textured Triangles");
        static_cast<kipod::GLRenderLayout*>(layout)->SetupGLTriangles(&triangles_);
    }else if(normals){
        auto layout = static_cast<kipod::GLRenderLayout*>(Layout("Colored Triangles"));
        layout->SetupColoredTriangles(&vertices_vector,&indices_vector,
                                               &normals_vector, &nindices_vector);
    }else{
        auto layout = static_cast<kipod::GLRenderLayout*>(Layout("Colored Triangles"));
        layout->SetupColoredTriangles(&vertices_vector,&indices_vector);
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


void MeshModel::CreateBoundingBox(){
    boundingBoxData_ = std::make_unique<BoundingBoxData>(vertices_vector);
}


glm::mat4 MeshModel::TansformBoundingBox()  {
    return Transform() * boundingBoxData_->transform_;
}

glm::vec3 MeshModel::Center(){
    glm::mat4 m = TransformWorld()*TransformLocal();
    glm::vec3 center = glm::vec3(m[3][0],m[3][1],m[3][2]);
	return center;
}

void MeshModel::CenterModel(){
    local_->Translate(-boundingBoxData_->center_);
}


void MeshModel::SetUniformMaterial()
{
    mat_ =  std::make_shared<kipod::RenderMaterial>();
}

}
