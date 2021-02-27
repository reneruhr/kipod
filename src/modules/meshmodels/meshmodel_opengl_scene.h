#pragma once
#include "../../kipod.h"

#include "meshmodel.h"
#include "meshmodel_primitive.h"


namespace kipod::MeshModels{

class MeshModelOpenGLScene :
        public kipod::Listener,
        public kipod::Controls,
        public kipod::RenderScene{
        friend class MeshmodelSidebar;
        friend class MeshModelModule;

        std::unordered_map<std::string, std::shared_ptr<kipod::Shader> > shaders_;


        kipod::GLRenderLayout&& CreateLayoutNormals(kipod::GLRenderLayout*);

        void BindMaterialUniforms(kipod::Shader& shader, const kipod::RenderMaterial &material);
        void BindLightUniforms(kipod::Shader& shader);
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


        std::unique_ptr<kipod::RenderObject> coordinate_axis_;
        std::unique_ptr<kipod::RenderObject> grid_;
        void DrawCoordinateAxis(kipod::RenderCamera* camera);
        void SetupCoordinateAxis();
        void DrawGrid(kipod::RenderCamera* camera);
        void SetupGrid();


        PrimMeshModel boundingBox;
        void DrawBoundingBox();

        bool mouse_rotation_active_ = false;

        void SetupOptions();


        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;

        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);

protected:
        virtual void Receive(std::shared_ptr<kipod::Event> event) override;

        void LoadOBJModel(std::string fileName, bool textures = false);
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

        MeshModelOpenGLScene(int width, int height)
            : RenderScene(width, height), boundingBox(Cube){ }
};

}
