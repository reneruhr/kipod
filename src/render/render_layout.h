#pragma once
#include "../core.h"
#include "render_buffer.h"
#include "render_shader.h"
#include "render_primitive.h"
#include "render_material.h"

namespace kipod{
class Texture;
class FrameBuffer;

class RenderLayout{
    friend class RenderObject;
public:
    virtual ~RenderLayout() = default;

    virtual void Draw()=0;
    virtual void Setup()=0;

    virtual void AddTo(const std::string& ,std::unordered_map<std::string, std::unique_ptr<RenderLayout>>&) = 0;
    virtual void ChangeTo(const std::string& ,std::unordered_map<std::string, std::unique_ptr<RenderLayout>>&){}
};






}
