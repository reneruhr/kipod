#pragma once
#include <string>
#include "../../../compute/opengl/compute_shader.h"

namespace kipod::ImageProcessing{


    struct DispatchGroupsSize{
        GLuint x=32;
        GLuint y=32;
        GLuint z=1;
    };

struct Algorithm{
    Algorithm() = default;
    Algorithm(std::string name) : name_(name) {}
    Algorithm(std::string name, std::string shader_name) : 
        name_(name), shader_(std::make_unique<ComputeShader>(shader_name)) {}
    std::string name_;
    void* data_ = nullptr;
    
    std::unique_ptr<Shader> shader_ = nullptr;
	std::unique_ptr<ComputeShader> compute_shader_ = nullptr;
	
    DispatchGroupsSize dispatch_groups_size_;
    GLuint tex_input;
    GLuint tex_output;
    
     void Setup() 
    {
         if(true){
		     shader_ = std::make_unique<Shader>("image_processing_loadimage.vert.glsl",
										"image_processing_loadimage.frag.glsl");
	     	shader_->AttachUniform<int>("width");
	     	shader_->AttachUniform<int>("height");
	     	shader_->AttachUniform<glm::mat3>("kernel_matrix");
	     	shader_->AttachUniform<int>("algorithm");
	     }
    }

    void Compute() 
    {
        compute_shader_->Use();
        //glBindImageTexture(0, tex_input, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGB);
        //glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGB);
        glBindImageTexture(3, tex_input, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGB);
     	glDispatchCompute(dispatch_groups_size_.x, dispatch_groups_size_.y, dispatch_groups_size_.z);
        compute_shader_->Unuse();
    }

	
    void SetupUniforms(Image* image, Kernel* kernel)
     {
        shader_->Use();
        glBindImageTexture(3, image->tex_->Id(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA);
        shader_->SetUniform<int>("width", image->Width());
        shader_->SetUniform<int>("height", image->Height());
        shader_->SetUniform<glm::mat3>("kernel_matrix", glm::mat3(*kernel));
        shader_->Unuse();
     }
	
    void Draw(Image* image)
    {
        shader_->Use();
        image->Draw();
        shader_->Unuse();
    }
};



}
