#pragma once
#include "glm/glm.hpp"
#include <iostream>
namespace kipod::ImageProcessing{

using KernelGenerator = glm::mat3(float s);

struct Kernel{
    glm::mat3 matrix_;
    std::string name_;
    float parameter_;
    KernelGenerator* generator_;

    Kernel() = default;
    virtual ~Kernel() = default;
    Kernel(Kernel&&) = default;
    Kernel& operator=(Kernel&&) = default;

    void Update(float s) {
        matrix_ = generator_(s);
        parameter_ = s;
    }
    void Update() {  Update(parameter_); }

    operator glm::mat3() {
        return matrix_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Kernel& kernel)
    {
        os << "[ ";
        for(int i = 0; i< 3; ++i){
            for(int j = 0; j< 3; ++j)
                os << kernel.matrix_[j][i] << ( j==2 ? "" : "," );
            os << ( i==2 ? " ]" : "\n" );
        }
        return os;
    }
};


struct IdentityKernel : Kernel{
   IdentityKernel(float s=1.0f){
        matrix_ = {};
        generator_ = [](float s){
            return glm::mat3(
                       0,0,0,
                       0,s,0,
                       0,0,0);
        };
        matrix_ = generator_(s);
        name_ = "Identity";
        parameter_ = s;
   }
};

struct LaplaceKernel : Kernel{
   LaplaceKernel(float s=1.0f){
       generator_ = [](float s){
           return glm::mat3(
                   -s,-s,-s,
                   -s,1+8*s,-s,
                   -s,-s,-s);
       };
       matrix_ = generator_(s);
       name_ = "Laplace";
       parameter_ = s;
   }
};

struct EdgeDetection : Kernel{
   EdgeDetection(float s=1.0f){
       generator_ = [](float s){
           return glm::mat3(
                   -s,-s,-s,
                   -s,8*s,-s,
                   -s,-s,-s);
       };
       matrix_ = generator_(s);
       name_ = "Edge Detection";
       parameter_ = s;
   }
};

struct Blur : Kernel{
   Blur(float s=1.0f){
       generator_ = [](float s){
           return glm::mat3(
                   s,2*s,s,
                   2*s,4*s,2*s,
                   s,2*s,s)/16.0f/s;
       };
       matrix_ = generator_(s);
       name_ = "Blur";
       parameter_ = s;
   }
};

struct Gaussian : Kernel{
   Gaussian(){
       generator_ = [](float){
           return glm::mat3(
                   1,2,1,
                   2,4,2,
                   1,2,1)/16.0f;
       };
       matrix_ = generator_(1);
       name_ = "Gaussian";
       parameter_ = 1.0f;
   }
};

struct Sobelx : Kernel{
   Sobelx(float s=1.0f){
       generator_ = [](float s){
           return glm::mat3(
                   -s,0,s,
                   -2*s,0,2*s,
                   -s,0,s);
       };
       matrix_ = generator_(s);
       name_ = "Sobel x";
       parameter_ = s;
   }
};

struct Sobely : Kernel{
   Sobely(float s=1.0f){
       generator_ = [](float s){
           return glm::mat3(
                   -s,-2*s,-s,
                   0,0,0,
                   s,2*s,s);
       };
       matrix_ = generator_(s);
       name_ = "Sobel y";
       parameter_ = s;
   }
};

}
