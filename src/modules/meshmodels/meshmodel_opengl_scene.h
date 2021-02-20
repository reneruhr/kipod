#pragma once
#include "../../kipod.h"

#include "../../meshmodel.h"
#include "../../primmeshmodel.h"


class MeshModelOpenGLScene :  public kipod::Listener, public kipod::Controls, public kipod::RenderScene{
friend class MeshmodelSidebar;
protected:
        std::unordered_map<std::string, kipod::Shader> shaders_;

        std::vector<MeshModel*> models;
        std::vector<kipod::RenderLight*> lights;
        std::vector<kipod::RenderCamera*> cameras;

        PrimMeshModel boundingBox;

        std::unique_ptr<kipod::RenderObject> coordinate_axis_;
        std::unique_ptr<kipod::RenderObject> grid_;

        void DrawBoundingBox();

        bool mouse_rotation_active_ = false;

        int activeModel=-1;
        int activeLight;
        int activeCamera=0;
public:
        MeshModelOpenGLScene(int width, int height)
            : RenderScene(width, height), boundingBox(Cube){}

        void LoadOBJModel(string fileName, bool textures = false);
        void LoadPrimitive(Primitive primitive, int numberPolygons=0);

        virtual void Setup() override;
        virtual void Draw() override;


        void SetupOptions();

        kipod::RenderCamera* GetActiveCamera();
        MeshModel* GetActiveModel();

        void AddCamera(kipod::RenderCamera *cam);


        void moveCamera(int camera_id, const vec3& eye, const vec3& at, const vec3& up );
        void moveEyeOfCamera(int camera_id, const vec3& eye);
        void perspectiveCamera(int camera_id, const float& fovy, const float& aspect, const float& near, const float& far);
        void moveModel(int model_id, const vec3& translate);
        void lookAtModel(int camera_id, int model_id);

        void AddLight(kipod::RenderLight *light);
        bool HasLight() { return !lights.empty(); }

        int numberOfCameras(){ return cameras.size();}
        int numberOfModels(){ return models.size();}
        bool HasModel() { return !models.empty(); }

        void SetActiveCamera(int id);
        void SetActiveModel(int id);



        void BindMaterialUniforms(kipod::Shader& shader, const kipod::RenderMaterial &material);
        void BindLightUniforms(kipod::Shader& shader, vector<kipod::RenderLight *> &lights);
        void BindMatrixUniforms(kipod::Shader& shader, const kipod::RenderObject &model, const kipod::RenderCamera &camera);
        void BindMatrixUniformsForMesh(kipod::Shader& shader, const MeshModel &model, const kipod::RenderCamera &camera);
        void BindTextureUniforms(kipod::Shader& shader, const kipod::Texture *texture);
        void BindNormalUniforms(kipod::Shader& shader, const float length);

        void SetUniform(vector<kipod::RenderLight *> &lights, kipod::RenderCamera *camera, MeshModel *model);
        void SetUniformNormal(MeshModel *model, kipod::RenderCamera *camera);
        void SetUniformTex(vector<kipod::RenderLight *> &lights, kipod::RenderCamera *camera, MeshModel *model);
        void SetUniformBox(MeshModel *model, kipod::RenderCamera* camera);


        void SetupShaders();
        void SetupShaderBasic();
        void SetupShaderNormals();
        void SetupShaderColoredTriangles();
        void SetupShaderTexturedTriangles();


        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;
        virtual void Signup() override;
        virtual void Receive(std::shared_ptr<kipod::Event> event) override;
        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);



        void DrawCoordinateAxis(kipod::RenderCamera* camera);
        void SetupCoordinateAxis();
        void DrawGrid(kipod::RenderCamera* camera);
        void SetupGrid();
};

