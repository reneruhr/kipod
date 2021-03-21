#pragma once
#include "../../core.h"
namespace kipod{
class SoftrendererBuffer
{
    std::shared_ptr< std::vector<vec3> > vertices_;
    std::shared_ptr< std::vector<unsigned int> > indices_;
    std::shared_ptr< std::vector<vec3> > normals_;
    std::shared_ptr< std::vector<unsigned int> > nindices_;

    unsigned int count_;
public:
    SoftrendererBuffer() = default;
    SoftrendererBuffer(std::shared_ptr< std::vector<vec3> > vertices,
                       std::shared_ptr< std::vector<unsigned int> > indices,
                       std::shared_ptr< std::vector<vec3> > normals = nullptr,
                       std::shared_ptr< std::vector<unsigned int> > nindices = nullptr) :
                        vertices_(vertices), indices_(indices),
                        normals_(normals), nindices_(nindices),
                        count_(indices_->size())
    {}

    vec4 Vertex(int triangle, int vertex)
    {
        return vec4((*vertices_)[(*indices_)[triangle+vertex]],1);
    }

    vec4 Normal(int triangle, int vertex)
    {
        return vec4((*normals_)[(*nindices_)[triangle+vertex]],0);
    }

    bool HasNormals(){
        return normals_->size()>0;
    }

    unsigned int Count(){
        return count_;
    }
};

}
