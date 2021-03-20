#pragma once
#include "../render_layout.h"
#include "softrenderer_buffer.h"

namespace kipod{

class SoftRenderer;
class SoftRenderLayout : public RenderLayout
{
    SoftRenderer* softrenderer_;
    std::shared_ptr<SoftrendererBuffer> buffer_;

public:
    SoftRenderLayout()=default;
    ~SoftRenderLayout() = default;
    virtual void Draw() override;
    virtual void Setup() override {}
    virtual void AddTo(const std::string&,
                       std::unordered_map<std::string, std::unique_ptr<RenderLayout>>&)
                        override;
    void SetSoftRenderer(SoftRenderer* softrenderer) {
        softrenderer_ = softrenderer;
    }

    void SetBuffer(std::vector<vec3>* vertices,
                   std::vector<unsigned int>* indices,
                   std::vector<vec3>* normals = nullptr,
                   std::vector<unsigned int>* nindices = nullptr)
    {
        buffer_ = std::make_shared<SoftrendererBuffer>(vertices, indices, normals, nindices);
    }

    SoftrendererBuffer& Buffer(){
        return *buffer_;
    }

};

}
