#pragma once
#include "../../../kipod.h"
#include "../meshmodel.h"
#include "../meshmodel_primitive.h"
#include "../meshmodel_API_scene.h"
#include "../../../render/softrenderer/softrenderer.h"

namespace kipod::MeshModels{

using MeshModel = kipod::MeshModels::MeshModel;

class MeshModelScene;
class SoftRendererScene  : public MeshModelAPIScene{
    friend class MeshmodelSidebar;
    friend class MeshModelModule;
    friend class MeshModelScene;

        std::shared_ptr<kipod::Shader> shader_;
        void SetupShader();


        std::unique_ptr<SoftRenderer> softrenderer_;

protected:
        void Setup() override;
        void Draw() override;

        void DrawBoundingBox(MeshModel *model, RenderCamera *camera) override;
        void DrawGrid(RenderCamera *camera) override;
        void DrawCoordinateAxis(RenderCamera *camera) override;

        void CreateMeshModelLayout(MeshModel *model) override;
        void CreatePrimitiveModelLayout(PrimMeshModel *model) override;
        void CreateCoordinateAxisLayout(std::vector<vec3> &vertices, std::vector<vec3> &colors) override;
        void CreateBoundingBoxLayout() override;
        void CreateGridLayout(std::vector<vec3> &vertices) override;
public:
        SoftRendererScene(MeshModelScene* scene);
};

}
