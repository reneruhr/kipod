#pragma once
#include "../../kipod.h"
#include "template.h"
namespace kipod::Template{


class TemplateScene :public kipod::Listener,
                     public kipod::Controls,
                     public kipod::RenderScene{
                     friend class TemplateSidebar;
                     friend class TemplateModule;

        using TemplateContainer = std::vector<std::unique_ptr<Template>>;
        TemplateContainer templates_;
        Template* active_template_ = nullptr;
        
        std::unordered_map<std::string, std::shared_ptr<kipod::Shader> > shaders_;
        void SetupLayout(Template* template);
        void SetupShaders();
        void SetupUniforms(Template* template);

        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;
        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);

protected:
        TemplateScene(int width, int height);

        virtual void Signup() override;
        virtual void Receive(std::shared_ptr<kipod::Event> event) override;
        virtual void Setup() override;
        virtual void Draw() override;


        TemplateContainer& Templates();

        void AddTemplate(Template&&);
        Template* ActiveTemplate();
        void ActiveTemplate(Template*);
        bool HasTemplate();

};

}
