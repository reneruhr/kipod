#include "softrenderer_framebuffer.h"

void kipod::SoftRendererFramebuffer::Clear(){
    for(int i = 0; i< width_*height_*3; i++) buffer_[i]=0;
}

float *kipod::SoftRendererFramebuffer::Data(){
    return buffer_.get();
}

void kipod::SoftRendererFramebuffer::DrawPoint(int x, int y)
{
    if( x < width_ && y < height_ && x>=0 && y>=0){
        buffer_[INDEX(width_,x,y,0)]=1;
        buffer_[INDEX(width_,x,y,1)]=1;
        buffer_[INDEX(width_,x,y,2)]=1;
    }
}

void kipod::SoftRendererFramebuffer::DrawStraightLine(int *p, int *q)
{


    if(p[0] == q[0] && p[0]<width_ && p[0]>=0){
        if(p[1] > q[1]) std::swap(p,q);

        for( int i = std::max(p[1],0); i<= std::min(q[1],height_-1) ; i++)
        {
            buffer_[INDEX(width_,p[0],i,0)]=1;
            buffer_[INDEX(width_,p[0],i,1)]=1;
            buffer_[INDEX(width_,p[0],i,2)]=1;
        }

    } else if(p[1] == q[1] && p[1]<height_ && p[1]>=0){
        if(p[0] > q[0]) std::swap(p,q);
        for( int i = std::max(p[0],0); i<= std::min(q[0],width_-1); i++)
        {
            buffer_[INDEX(width_,i,p[1],0)]=1;
            buffer_[INDEX(width_,i,p[1],1)]=1;
            buffer_[INDEX(width_,i,p[1],2)]=1;
        }
    }
}

void kipod::SoftRendererFramebuffer::DrawPointWithColor(int x, int y, float *c)
{
    buffer_[INDEX(width_,x,y,0)]=*c;
    buffer_[INDEX(width_,x,y,1)]=*(c+1);
    buffer_[INDEX(width_,x,y,2)]=*(c+2);
}
