#pragma once
#include <memory>
#include <algorithm>

namespace kipod{

class RaytracerFramebuffer
{
    std::unique_ptr<float[]> buffer_;
    int INDEX(int width_,int x,int y,int c){
        return (x+y*width_)*3+c;
    }
    int width_;
    int height_;
public:
    RaytracerFramebuffer(int width, int height) :
        buffer_(std::make_unique<float[]>(width*height*3) ),
        width_(width), height_(height){}

    void Clear(){
        for(int i = 0; i< width_*height_*3; i++) buffer_[i]=0;
    }

    float* Data(){
        return buffer_.get();
    }

    void DrawPoint(int x, int y, float* c)
    {
        buffer_[INDEX(width_,x,y,0)]=*c;
        buffer_[INDEX(width_,x,y,1)]=*(c+1);
        buffer_[INDEX(width_,x,y,2)]=*(c+2);
    }
};

}
