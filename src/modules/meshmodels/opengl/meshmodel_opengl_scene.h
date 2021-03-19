#pragma once
#include "../../../kipod.h"

#include "../meshmodel.h"
#include "../meshmodel_primitive.h"


namespace kipod::MeshModels{

class MeshModelScene;
class MeshModelOpenGLScene {
        friend class MeshmodelSidebar;
        friend class MeshModelModule;
        friend class MeshModelScene;

        MeshModelScene* scene_;
        std::unordered_map<std::string, std::shared_ptr<kipod::Shader> > shaders_;

        void CreateMeshModelLayout();

        kipod::GLRenderLayout&& CreateLayoutNormals(kipod::GLRenderLayout*);

        void BindMaterialUniforms(kipod::Shader& shader, const kipod::RenderMaterial &material);
        void BindMatrixUniforms(kipod::Shader& shader, const kipod::RenderObject &model, const kipod::RenderCamera &camera);
        void BindTextureUniforms(kipod::Shader& shader, const kipod::Texture *texture);
        void BindNormalUniforms(kipod::Shader& shader, const float length);

        void SetUniform(kipod::RenderCamera *camera, kipod::RenderObject *model);
        void SetUniformNormal(MeshModel *model, kipod::RenderCamera *camera);
        void SetUniformTex(kipod::RenderCamera *camera, kipod::RenderObject *model);
        void SetUniformBox(MeshModel *model, kipod::RenderCamera* camera);

        void SetupShaders();
        void SetupShaderBasic();
        void SetupShaderNormals();
        void SetupShaderColoredTriangles();
        void SetupShaderTexturedTriangles();

        void DrawBoundingBox();
        void SetupOptions();

protected:
        void Setup() ;
        void Draw();

        void CreateMeshModelLayout(MeshModel *model);
        void CreatePrimitiveModelLayout(PrimMeshModel *model);
        void DrawGrid(RenderObject *grid, kipod::RenderCamera *camera);
        void CreateGridLayout(RenderObject *grid, std::vector<vec3> &vertices);
        void DrawCoordinateAxis(RenderObject *coordinate_axis, kipod::RenderCamera *camera);
        void CreateCoordinateAxisLayout(RenderObject *coordinate_axis, std::vector<vec3> &vertices, std::vector<vec3> &colors);
        void CreateBoundingBoxLayout(PrimMeshModel *bounding_box);
        void BindLightUniforms(kipod::Shader &shader);

public:
       MeshModelOpenGLScene(MeshModelScene* scene): scene_(scene) {};
};

}
