#pragma once
#include "../../../kipod.h"
#include "../meshmodel.h"
#include "../meshmodel_primitive.h"
#include "../meshmodel_API_scene.h"
#include "../../../render/raytracer/raytracer.h"

namespace kipod::MeshModels{

using MeshModel = kipod::MeshModels::MeshModel;

class MeshModelScene;
class RaytracerScene  : public MeshModelAPIScene{
    friend class MeshmodelSidebar;
    friend class MeshModelModule;
    friend class MeshModelScene;


        std::unique_ptr<Raytracer> raytracer_;
        glm::vec3 Screen(int i,int j);


protected:
        void Setup() override;
        void Draw() override;
        void Resize(int, int) override;

        void DrawBoundingBox(MeshModel *model, RenderCamera *camera) override {}
        void DrawGrid(RenderCamera *camera) override{}
        void DrawCoordinateAxis(RenderCamera *camera) override{}

        void CreateCoordinateAxisLayout(std::vector<vec3> &vertices,
                                        std::vector<vec3> &colors) override{}
        void CreateBoundingBoxLayout() override{}
        void CreateGridLayout(std::vector<vec3> &vertices) override{}
        void CreateMeshModelLayout(MeshModel *model) override{}


        void CreatePrimitiveModelLayout(PrimMeshModel *model) override;
public:
        RaytracerScene(MeshModelScene* scene);
};

}
