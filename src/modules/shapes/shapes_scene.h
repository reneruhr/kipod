#pragma once
#include "../../kipod.h"
#include "shape.h"


namespace kipod::Shapes{

class ShapesScene :  public kipod::Listener, public kipod::Controls, public kipod::RenderScene{
friend class ShapesSidebar;
friend class ShapesModule;

        std::unordered_map<std::string, std::shared_ptr<kipod::Shader> > shaders_;

        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override {};
        virtual void Signup() override {};
        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);

protected:
        virtual void Receive(std::shared_ptr<kipod::Event> event) override {};
        virtual void Setup() override {};
        virtual void Draw() override {};

        ShapesScene(int width, int height): RenderScene(width, height) {}
};

}
