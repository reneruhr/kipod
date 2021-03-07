#pragma once
#include "../../kipod.h"
#include "softrenderer.h"
#include "../meshmodels/meshmodel.h"

namespace kipod::SoftRenderer{

using MeshModel = kipod::MeshModels::MeshModel;

class SoftRendererScene :   public kipod::Listener,
                            public kipod::Controls,
                            public kipod::RenderScene{
                            friend class SoftRendererSidebar;
                            friend class SoftRendererModule;

        using MeshmodelContainer = std::vector<std::unique_ptr<MeshModel>>;
        MeshmodelContainer meshmodels_;
        MeshModel* active_meshmodel_ = nullptr;

        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;
        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);


        std::shared_ptr<kipod::Shader> shader_;
public:
        void SetupLayout(MeshModel *meshmodel);
        void SetupShader();
protected:
        SoftRendererScene(int width, int height);

        virtual void Signup() override;
        virtual void Receive(std::shared_ptr<kipod::Event> event) override;
        virtual void Setup() override;
        virtual void Draw() override;


        MeshmodelContainer& MeshModels();

        void AddMeshModel(MeshModel&&);
        MeshModel* ActiveMeshModel();
        void ActiveMeshModel(MeshModel*);
        bool HasMeshModel();

};

}
