#include "image_processing_scene.h"

void kipod::ImageProcessing::ImageProcessingScene::Setup()
{
        name_ = "ImageProcessing Scene";

        SetupKernels();
        SetupShaders();
        Signup();

        LOG_INFO("ImageProcessing Scene Initialized.");
        LOG_CONSOLE("ImageProcessing Scene Initialized.");
}

void kipod::ImageProcessing::ImageProcessingScene::Draw()
{
    framebuffer_->Bind();
    glViewport(0, 0, width_, height_);
    glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    auto shader = shaders_["ImageProcessing"];
    shader->Use();
    for(auto& image : images_){
        SetupUniforms(image.get(), ActiveKernel());
        image->Draw();
    }
    shader->Unuse();
    glDisable(GL_DEPTH_TEST);

    kipod::RenderManager::Bind(0);

}

void kipod::ImageProcessing::ImageProcessingScene::LoadImage(std::filesystem::path path)
{
        Image *image = new Image(path);        
        reverse_ratio_ = image->Height() * 1.0f / image->Width();
        Resize(image->Width(), image->Height());
        SetupLayout(image);
        AddImage(std::move(*image));
        active_image_ = images_.back().get();
}

void kipod::ImageProcessing::ImageProcessingScene::AddImage(Image&& image)
{
    images_.push_back(
                std::make_unique<Image>(
                    std::forward<Image>(image)));
}

void kipod::ImageProcessing::ImageProcessingScene::ActiveImage(
        Image* active)
{
    active_image_= active;
}

kipod::ImageProcessing::Image* kipod::ImageProcessing::ImageProcessingScene::ActiveImage()
{
    return active_image_;
}


bool kipod::ImageProcessing::ImageProcessingScene::HasImage()
{
    return active_image_!=nullptr;
}

void kipod::ImageProcessing::ImageProcessingScene::AddKernel(kipod::ImageProcessing::Kernel &&kernel)
{
    kernels_.push_back(
                std::make_unique<Kernel>(
                    std::forward<Kernel>(kernel)));
}

kipod::ImageProcessing::Kernel *kipod::ImageProcessing::ImageProcessingScene::ActiveKernel()
{
    return active_kernel_;
}

void kipod::ImageProcessing::ImageProcessingScene::ProcessKeys(kipod::KeyPressedEvent &event)
{
    float stepsize = 1.0f;
    auto key = event.GetKeyCode();

    if(HasImage()){
        if(key == Key::Left){
                    ActiveImage()->world_->Translate({-stepsize,0,0});
                }
        else if(key == Key::Right){
                    ActiveImage()->world_->Translate({+stepsize,0,0});
                }
        else if(key == Key::Up){
                    ActiveImage()->world_->Translate({0,stepsize,0});
                }
        else if(key == Key::Down){
                    ActiveImage()->world_->Translate({0,-stepsize,0});
                }
        else if(key == Key::PageUp){
                    ActiveImage()->world_->Translate({0,0, stepsize});
                }
        else if(key == Key::PageDown){
                    ActiveImage()->world_->Translate({0,0,-stepsize});
                }
    }
}

void kipod::ImageProcessing::ImageProcessingScene::SetupKernels()
{
    AddKernel(IdentityKernel());
    active_kernel_ = kernels_[0].get();
    AddKernel(LaplaceKernel());
}

void kipod::ImageProcessing::ImageProcessingScene::SetupLayout(Image* image)
{
   image->Init();
}

void kipod::ImageProcessing::ImageProcessingScene::SetupShaders()
{
    shaders_.insert({"ImageProcessing", std::make_shared<kipod::Shader>("image_processing.vert.glsl",   "image_processing.frag.glsl")});
    auto shader = shaders_["ImageProcessing"];

    shader->AttachUniform<int>("tex");
    shader->AttachUniform<int>("width");
    shader->AttachUniform<int>("height");
    shader->AttachUniform<glm::mat3>("kernel_matrix");
}

void kipod::ImageProcessing::ImageProcessingScene::SetupUniforms(Image *image, Kernel* kernel)
{
    auto shader = shaders_["ImageProcessing"];
    shader->SetUniform<int>(image->tex_->name_.c_str(), 0);
    shader->SetUniform<int>("width", image->Width());
    shader->SetUniform<int>("height", image->Height());
    shader->SetUniform<glm::mat3>("kernel_matrix", glm::mat3(*kernel));
    //LOG_DEBUG("Kernel used: {}", *kernel );
}


kipod::ImageProcessing::ImageProcessingScene::ImageProcessingScene(int width, int height):
    RenderScene(width, height), images_(){}

void kipod::ImageProcessing::ImageProcessingScene::Signup()
{
    kipod::Events::Signup(this, kipod::EventCategoryKeyboard); LOG_CATEGORY_NAME(kipod::EventCategoryKeyboard);
}

void kipod::ImageProcessing::ImageProcessingScene::Receive(std::shared_ptr<kipod::Event> event)
{
    Process<kipod::KeyPressedEvent>(event, BIND_EVENT_FN(ImageProcessingScene::ProcessKeys));
}
