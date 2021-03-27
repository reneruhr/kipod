#pragma once
#include "../../../kipod.h"

#include "../meshmodel.h"
#include "../meshmodel_primitive.h"
#include "../meshmodel_API_scene.h"

namespace kipod::MeshModels{

class MeshModelScene;
class OpenGLScene : public MeshModelAPIScene{
        friend class MeshmodelSidebar;
        friend class MeshModelModule;
        friend class MeshModelScene;

        kipod::GLRenderLayout&& CreateLayoutNormals(kipod::GLRenderLayout*);

        void BindMaterialUniforms(kipod::Shader& shader, const kipod::RenderMaterial &material);
        void BindMatrixUniforms(kipod::Shader& shader, const kipod::RenderObject &model, const kipod::RenderCamera &camera);
        void BindTextureUniforms(kipod::Shader& shader, const kipod::Texture *texture);
        void BindNormalUniforms(kipod::Shader& shader, const float length);
        void BindLightUniforms(kipod::Shader& shader);

        void SetUniform(kipod::RenderCamera *camera, kipod::RenderObject *model);
        void SetUniformNormal(MeshModel *model, kipod::RenderCamera *camera);
        void SetUniformTex(kipod::RenderCamera *camera, kipod::RenderObject *model);
        void SetUniformBox(MeshModel *model, kipod::RenderCamera* camera);

        void SetUniformCamera(kipod::RenderCamera* cameraModel, kipod::RenderCamera* camera);
        void SetUniformFrustum(kipod::RenderCamera *cameraModel, kipod::RenderCamera *camera);
        void SetUniformLight(kipod::RenderLight *light, kipod::RenderCamera *camera);

        void SetupShaders();
        void SetupShaderBasic();
        void SetupShaderNormals();
        void SetupShaderColoredTriangles();
        void SetupShaderTexturedTriangles();

protected:
        void Setup() override;
        void Draw() override;

        void CreateMeshModelLayout(MeshModel *model) override;
        void CreatePrimitiveModelLayout(PrimMeshModel *model) override;
        void CreateBoundingBoxLayout() override;

        void CreateBoundingSphereLayout();

        void CreateGridLayout(std::vector<vec3> &vertices) override;
        void CreateCoordinateAxisLayout(std::vector<vec3> &vertices, std::vector<vec3> &colors) override;

        void DrawCamera(RenderCamera *camera_model, RenderCamera *camera);
        void DrawFrustum(RenderCamera *camera_model, RenderCamera *camera);
        void DrawLight(RenderLight *light, RenderCamera *camera);

        void DrawBoundingBox(MeshModel *model, RenderCamera *camera)  override;
        void DrawGrid(RenderCamera *camera) override;
        void DrawCoordinateAxis(RenderCamera *camera) override;

public:
        OpenGLScene(MeshModelScene* scene): MeshModelAPIScene(scene) {};


};
}
