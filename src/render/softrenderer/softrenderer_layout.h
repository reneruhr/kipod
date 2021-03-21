#pragma once
#include "../render_layout.h"
#include "softrenderer_buffer.h"

namespace kipod{

class SoftRenderer;
class SoftRenderLayout : public RenderLayout
{
    SoftRenderer* softrenderer_;
    std::unique_ptr<SoftrendererBuffer> buffer_;

public:
    SoftRenderLayout()=default;
    ~SoftRenderLayout() = default;
    SoftRenderLayout(SoftRenderLayout&&)=default;
    SoftRenderLayout& operator=(SoftRenderLayout&&)=default;
    virtual void Draw() override;
    virtual void Setup() override {}
    virtual void AddTo(const std::string&,
                       std::unordered_map<std::string, std::unique_ptr<RenderLayout>>&)
                        override;
    void SetSoftRenderer(SoftRenderer* softrenderer) {
        softrenderer_ = softrenderer;
    }

    void SetBuffer(std::shared_ptr< std::vector<vec3> > vertices,
                   std::shared_ptr< std::vector<unsigned int> > indices,
                   std::shared_ptr< std::vector<vec3> > normals = nullptr,
                   std::shared_ptr< std::vector<unsigned int> > nindices = nullptr)
    {
        buffer_ = std::make_unique<SoftrendererBuffer>(vertices, indices, normals, nindices);
    }

    SoftrendererBuffer& Buffer(){
        return *buffer_;
    }

};

}
