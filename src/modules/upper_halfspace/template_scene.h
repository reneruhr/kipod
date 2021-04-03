#pragma once
#include "../../kipod.h"
#include "template.h"
namespace kipod::Template{

class TemplateObject : public kipod::RenderObject 
{

class TemplateData;

public:
    std::shared_ptr<TemplateData> template_data_ = nullptr;
    void Init();

    TemplateObject() = default;
    TemplateObject(std::filesystem::path path);
    virtual ~TemplateObject(void);
    TemplateObject(TemplateObject&&) = default;
    TemplateObject& operator=(TemplateObject&&) = default;

};

}


class TemplateScene :public kipod::Listener,
                     public kipod::Controls,
                     public kipod::RenderScene{
                     friend class TemplateSidebar;
                     friend class TemplateModule;

        using TemplateObjectsContainer = std::vector<std::unique_ptr<TemplateObject>>;
        TemplateContainer templates_;
        TemplateObject* active_template_object_ = nullptr;
        
        std::unordered_map<std::string, std::shared_ptr<kipod::Shader> > shaders_;
        void SetupLayout(TemplateObject*);
        void SetupShaders();
        void SetupUniforms(TemplateObject*);

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

        void AddTemplateObject(TemplateObject&&);
        Template* ActiveTemplateObject();
        void ActiveTemplateObject(Template*);
        bool HasTemplateObject();

};

}
