#include "../../core.h"


#include "meshmodel.h"
#include "../../render/render_texture.h"
#include "meshmodel_parsing.h"

namespace kipod::MeshModels{

MeshModel::MeshModel(std::string fileName, bool textured)
{
    name_ = fileName;
    LoadFile(fileName,
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

        auto end = std::end(fileName);
        *(end-3) = 'p'; *(end-2) = 'n'; *(end-1) = 'g';
        if(std::filesystem::exists(fileName)){
            tex_->LoadTexture(fileName.c_str());
            return;
         }
        *(end-3) = 'j'; *(end-2) = 'p'; *(end-1) = 'g';
        if(std::filesystem::exists(fileName)){
            tex_->LoadTexture(fileName.c_str());
            return;
         }
        *(end-3) = 's'; *(end-2) = 'v'; *(end-1) = 'g';
        if(std::filesystem::exists(fileName)){
            tex_->LoadTexture(fileName.c_str());
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
