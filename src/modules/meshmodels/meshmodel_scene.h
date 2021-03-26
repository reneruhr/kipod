#pragma once
#include "../../kipod.h"

#include "meshmodel.h"
#include "meshmodel_primitive.h"
#include "opengl/meshmodel_opengl_scene.h"
#include "softrenderer/softrenderer_scene.h"

namespace kipod::MeshModels{
class OpenGLScene;
class SoftRendererScene;

class MeshModelScene :
        public kipod::Listener,
        public kipod::Controls,
        public kipod::RenderScene {
        friend class MeshmodelSidebar;
        friend class MeshModelModule;
        friend class OpenGLScene;
        friend class SoftRendererScene;

protected:
        std::unique_ptr<OpenGLScene> opengl_impl_;
        std::unique_ptr<SoftRendererScene> softrenderer_impl_;
        bool needs_update_ = false;
        void NeedsUpdate() { needs_update_ = true;}

        std::unique_ptr<kipod::RenderObject> coordinate_axis_;
        std::unique_ptr<kipod::RenderObject> grid_;
        PrimMeshModel bounding_box_;

        bool mouse_rotation_active_ = false;

        void SetupOptions();
        void SetupCoordinateAxis();
        void SetupGrid();


        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;

        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);


        virtual void Receive(std::shared_ptr<kipod::Event> event) override;

        void LoadOBJModel(std::filesystem::path path, bool textures = false);
        void LoadPrimitive(Primitive primitive, int numberPolygons=0);

        MeshModel* GetActiveModel();
        void AddModel(MeshModel&&);
        bool HasModel() { return HasRenderObject(); }
        int NumberOfModels(){ return NumberOfRenderObjects();}
        void SetActiveModel(int id);
        void LookAtModel(int camera_id, int model_id);

        virtual void Signup() override;
        virtual void Setup() override;
        virtual void Draw() override;

        MeshModelScene(int width, int height)
            : RenderScene(width, height), bounding_box_(Cube){ }
};

}
