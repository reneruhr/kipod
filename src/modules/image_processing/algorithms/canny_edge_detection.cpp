#include "canny_edge_detection.h"
#include "glm/glm.hpp"
#include <utility>
#include "../kernel.h"
#include <cmath>


namespace kipod::ImageProcessing{

// Did not test! Used to outline shader program.

float Convolution(const glm::mat3& kernel, const glm::mat3& sample)
{
    float pixel = 0.0f;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            pixel+= kernel[j][i]*sample[j][i];
    return pixel;
}


float RGBtoGray(const glm::vec3& rgb)
{
    return 0.299*rgb.r + 0.587*rgb.g +0.114*rgb.b;
}

float ApplyGaussian(const glm::mat3& pixel_sample)
{
    Gaussian kernel;
    return Convolution(kernel.matrix_, pixel_sample);
}

std::pair<float,int> ApplySobel(const glm::mat3& pixel_sample)
{
    Sobelx sobelx;
    Sobely sobely;

    float Dx = Convolution(sobelx.matrix_, pixel_sample);
    float Dy = Convolution(sobely.matrix_, pixel_sample);

    float theta = atan(Dy/Dx);
    int partition;

    float division = M_PI/16.0f;

    if(theta <= division && theta > -division) partition = 0;
    else if(theta+M_PI/8 <= division && theta+M_PI/8 > -division) partition = 1;
    else if(theta-M_PI/8 <= division && theta-M_PI/8 > -division) partition = 2;
    else if(theta+M_PI/4 <= division && theta+M_PI/4 > -division) partition = 3;

    return {sqrt(Dx*Dx+Dy*Dy), partition};
}

float Thinning(const glm::mat3& magnitude_sample, int partition){
    float point = magnitude_sample[1][1];
    switch(partition){
    case 0:
        if(point < magnitude_sample[0][1] || point < magnitude_sample[2][1]) point = 0;
        break;
    case 1:
        if(point < magnitude_sample[0][0] || point < magnitude_sample[2][2]) point = 0;
        break;
    case 2:
        if(point < magnitude_sample[2][0] || point < magnitude_sample[0][2]) point = 0;
        break;
    case 3:
        if(point < magnitude_sample[1][0] || point < magnitude_sample[1][2]) point = 0;
        break;
    }
    return point;
}

float Treshhold(const glm::mat3& magnitude_sample, int low, int high){
    float point = magnitude_sample[1][1];
    if(point < low) return 0;
    else if (point > high) return point;
    else{
        for(int i=0; i<3; i+=2)
            for(int j=0; j<3; j+=2)
                if(magnitude_sample[j][i] > high) return point;
    }
    return 0;
}

}
