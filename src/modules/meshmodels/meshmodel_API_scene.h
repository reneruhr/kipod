#pragma once
#include "../../kipod.h"

#include "meshmodel.h"
#include "meshmodel_primitive.h"


namespace kipod::MeshModels{

class MeshModelScene;
class MeshModelAPIScene {
        friend class MeshmodelSidebar;
        friend class MeshModelModule;
        friend class MeshModelScene;
protected:
        MeshModelScene* scene_;
        std::unordered_map<std::string, std::shared_ptr<kipod::Shader> > shaders_;

        virtual void Setup() = 0;
        virtual void PrepareScreen() = 0;
        virtual void Resize(int w, int h) = 0;

        virtual void CreateMeshModelLayout(MeshModel *model) = 0;
        virtual void CreatePrimitiveModelLayout(PrimMeshModel *model) = 0;
        virtual void CreateBoundingBoxLayout() = 0;

        virtual void CreateGridLayout(std::vector<Vec3> &vertices) = 0;
        virtual void CreateCoordinateAxisLayout(std::vector<Vec3> &vertices,
                                                std::vector<Vec3> &colors) = 0;

        virtual void DrawBoundingBox(MeshModel *model, RenderCamera *camera)  = 0;
        virtual void DrawGrid(RenderCamera *camera) = 0;
        virtual void DrawCoordinateAxis(RenderCamera *camera) = 0;

public:
       virtual void Draw() = 0;
       MeshModelAPIScene(MeshModelScene* scene): scene_(scene) {};
       virtual ~MeshModelAPIScene() {}
};

}
