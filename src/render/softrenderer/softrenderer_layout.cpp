#include "softrenderer_layout.h"

namespace kipod{

void SoftRenderLayout::Draw()
{
//    void DrawTriangles(SoftrendererBuffer& buffer, mat4& model,
//                       mat4& camera,
//                       bool wireframeMode = true, bool clippingMode=true);
//    void DrawColoredTriangles(SoftrendererBuffer& buffer,
//                              mat4& model,
//                              RenderCamera& camera,
//                              const std::vector<RenderMaterial> *colors, const std::vector<unsigned int> *cindices,
//                              const std::vector<RenderLight*> &lights,
//                              bool lightMode=true, bool emissiveMode=false);

}

void SoftRenderLayout::AddTo(const std::string &name, std::unordered_map<std::string, std::unique_ptr<RenderLayout> > &map)
{
    map.insert( { name, std::make_unique<SoftRenderLayout>(std::move(*this)) } );
}



}
