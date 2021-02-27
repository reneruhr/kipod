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



        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;

        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);

        void SetupLayout(Shape* shape);
        void SetupShaders();
        void SetupUniforms(Shape* shape);

        void SetupGrid2d();
        void SetupOptions();

protected:
        ShapesScene(int width, int height);

        virtual void Signup() override;
        virtual void Receive(std::shared_ptr<kipod::Event> event) override;
        virtual void Setup() override;
        virtual void Draw() override;

        std::multiset<std::unique_ptr<Shape>,
                      std::function<bool(const std::unique_ptr<Shape>&,
                                         const std::unique_ptr<Shape>&)> > shapes_;
        std::multiset<std::unique_ptr<Shape>>::iterator active_shape_ = std::end(shapes_);

        void AddShape(Shape&&);
        Shape* ActiveShape();
        void ActiveShape(std::multiset<std::unique_ptr<Shape>>::iterator);
        bool HasShape();

};

}
