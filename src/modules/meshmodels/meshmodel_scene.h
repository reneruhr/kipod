#pragma once
#include "../../kipod.h"
#include "meshmodel.h"
#include "meshmodel_primitive.h"
#include "opengl/meshmodel_opengl_scene.h"
#include "softrenderer/softrenderer_scene.h"
#include "raytracer/raytracer_scene.h"

namespace kipod::MeshModels{
class OpenGLScene;
class SoftRendererScene;

class MeshModelScene :
        public kipod::Listener,
        public kipod::Controls,
        public kipod::RenderScene {
        friend class MeshmodelSidebar;
        friend class MeshModelModule;
        friend class MeshModelModuleMenu;
        friend class OpenGLScene;
        friend class SoftRendererScene;
        friend class RaytracerScene;

protected:
        std::unique_ptr<OpenGLScene> opengl_impl_;
        std::unique_ptr<SoftRendererScene> softrenderer_impl_;
        std::unique_ptr<RaytracerScene> raytracer_impl_;
        bool needs_update_ = false;
        void NeedsUpdate() { needs_update_ = true;}

        std::unique_ptr<kipod::RenderObject> coordinate_axis_;
        std::unique_ptr<kipod::RenderObject> grid_;
        PrimMeshModel bounding_box_;
        PrimMeshModel bounding_sphere_;

        bool mouse_rotation_active_ = false;

        void SetupOptions();
        void SetupKeys();
        void SetupCoordinateAxis();
        void SetupGrid();


        void ProcessKeys(kipod::KeyPressedEvent& event) override;

        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);


        void Receive(std::shared_ptr<kipod::Event> event) override;

        void LoadOBJModel(std::filesystem::path path, bool textures = false);
        void LoadPrimitive(Primitive primitive, int numberPolygons=0);
        void LoadPrimitive(Quadric);

        MeshModel* GetActiveModel();
        void AddModel(const PrimMeshModel&);
        bool HasModel() { return HasRenderObject(); }
        int NumberOfModels(){ return NumberOfRenderObjects();}
        void SetActiveModel(int id);
        void LookAtModel(int camera_id, int model_id);

        void Setup() override;
        void Resize(int w, int h) override;

        MeshModelScene(int width, int height)
            : RenderScene(width, height), bounding_box_(Primitive::Cube), bounding_sphere_(Primitive::Sphere, 5){ }

public:
        void Signup() override;
        void Draw() override;
        auto AddModel(std::filesystem::path path, bool textures = false) -> MeshModel*;
        auto AddModel(std::unique_ptr<MeshModel> model, bool visible = true) -> MeshModel*;
        auto AddModel(const MeshModel& model) -> MeshModel*;
        void SetActiveModel(MeshModel* model, bool set_explicitly_visible = false);
};

}
