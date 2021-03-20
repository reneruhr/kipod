#pragma once
#include "../../core.h"
namespace kipod{
class SoftrendererBuffer
{
    std::vector<vec3>* vertices_;
    std::vector<unsigned int>* indices_;
    std::vector<vec3>* normals_;
    std::vector<unsigned int>* nindices_;

    unsigned int count_;
public:
    SoftrendererBuffer() = default;
    SoftrendererBuffer(std::vector<vec3>* vertices,
                       std::vector<unsigned int>* indices,
                       std::vector<vec3>* normals = nullptr,
                       std::vector<unsigned int>* nindices = nullptr) :
                        vertices_(vertices), indices_(indices),
                        normals_(normals), nindices_(nindices),
                        count_(indices_->size())
    {}

    vec4&& Vertex(int triangle, int vertex)
    {
        return std::move(vec4((*vertices_)[(*indices_)[triangle+vertex]],1));
    }

    vec4&& Normal(int triangle, int vertex)
    {
        return std::move(vec4((*normals_)[(*nindices_)[triangle+vertex]],0));
    }

    bool HasNormals(){
        return normals_;
    }

    unsigned int Count(){
        return count_;
    }
};

}
