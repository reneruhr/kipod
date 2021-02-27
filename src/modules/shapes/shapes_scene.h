#pragma once
#include "../../kipod.h"
#include "shape.h"
namespace kipod::Shapes{


class ShapesScene :  public kipod::Listener,
                     public kipod::Controls,
                     public kipod::RenderScene{
                     friend class ShapesSidebar;
                     friend class ShapesModule;

        std::unordered_map<std::string, std::shared_ptr<kipod::Shader> > shaders_;

        std::multiset<std::unique_ptr<Shape>,
        std::function<bool(const std::unique_ptr<Shape>&, const std::unique_ptr<Shape>&)> > shapes_;

        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override {};
        virtual void Signup() override {};
        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);

        void SetupLayout(Shape* shape);
        void SetupShaders();
        void SetupUniforms(Shape* shape);

        void SetupGrid2d();
        void SetupOptions();

protected:
        ShapesScene(int width, int height):
            RenderScene(width, height),
            shapes_([](const std::unique_ptr<Shape>& x, const std::unique_ptr<Shape>& y)
                        {
                            return x->depth_ < y->depth_;
                        }
            ){}

        virtual void Receive(std::shared_ptr<kipod::Event> event) override {};
        virtual void Setup() override;
        virtual void Draw() override;

        void AddShape(Shape&&);

};

}
