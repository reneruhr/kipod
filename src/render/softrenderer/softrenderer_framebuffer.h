#pragma once
#include <memory>
#include <algorithm>

namespace kipod{

class SoftRendererFramebuffer
{
    std::unique_ptr<float[]> buffer_;
    int INDEX(int width_,int x,int y,int c){
        return (x+y*width_)*3+c;
    }
    int width_;
    int height_;
public:
    SoftRendererFramebuffer(int width, int height) :
        buffer_(std::make_unique<float[]>(width*height*3) ),
        width_(width), height_(height){}

    void Clear();
    float* Data();
    void DrawPoint(int x, int y);
    void DrawStraightLine(int *p, int *q);
    void DrawPointWithColor(int x, int y, float* c);
};

}
