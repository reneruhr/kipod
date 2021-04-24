#pragma once
#include "../../kipod.h"
#include "image.h"
#include "kernel.h"
#include "algorithms/image_algorithms.h"

namespace kipod::ImageProcessing{


using Image = kipod::ImageProcessing::Image;
using Images = std::vector<std::unique_ptr<Image>>;
using Kernels = std::vector<std::unique_ptr<Kernel>>;
using Algorithms = std::vector<Algorithm>;

using Shaders = std::unordered_map<std::string, std::shared_ptr<kipod::Shader> >;

class ImageProcessingScene :public kipod::Listener,
                     public kipod::Controls,
                     public kipod::RenderScene{
                     friend class ImageProcessingSidebar;
                     friend class ImageProcessingModule;

        Images images_;
        Image* active_image_ = nullptr;
        Kernels kernels_;
        Kernel* active_kernel_ = nullptr;

        Algorithms algorithms_;
        Algorithm* active_algorithm_ = nullptr;
        
        Shaders shaders_;

        void SetupAlgorithms();
        void SetupKernels();
        void SetupLayout(Image*);
        void SetupShaders();
        void SetupUniforms(Image*,  Kernel* kernel, Algorithm* algorithm=nullptr);

        void ClearAlgorithms();

        virtual void ProcessKeys(kipod::KeyPressedEvent& event) override;
        void ProcessMouseButtons(kipod::MouseButtonEvent& event);
        void ProcessMouseMoves(kipod::MouseMoveEvent& event);

protected:
        ImageProcessingScene(int width, int height);

        virtual void Signup() override;
        virtual void Receive(std::shared_ptr<kipod::Event> event) override;
        virtual void Setup() override;
        virtual void Draw() override;


        Images& ImageProcessing();

        void LoadImage(std::filesystem::path path);

        void AddImage(Image&&);
        Image* ActiveImage();
        void ActiveImage(Image*);
        bool HasImage();

        void AddKernel(Kernel&&);
        Kernel* ActiveKernel();

        Algorithm* ActiveAlgorithm() { return active_algorithm_;}
public:
};

}
