#pragma once
#include "../render_layout.h"

namespace kipod{


enum class RaytracerPrimitive{
    Sphere
};


struct RaytracerSphere{
    float radius;
    float center[3];
};


struct RaytracerBuffer{
    RaytracerBuffer(RaytracerPrimitive type, void* data) : type_(type), data_(data){}
    RaytracerPrimitive type_;
    void* data_;
};


class Raytracer;
class RaytracerLayout : public RenderLayout
{
    Raytracer* raytracer_;
    std::unique_ptr<RaytracerBuffer> buffer_;
    

public:
    RaytracerLayout()=default;
    ~RaytracerLayout() = default;
    RaytracerLayout(RaytracerLayout&&)=default;
    RaytracerLayout& operator=(RaytracerLayout&&)=default;
    virtual void Draw() override {};
    virtual void Setup() override {}
    virtual void AddTo(const std::string &name, std::unordered_map<std::string, std::unique_ptr<RenderLayout> > &map) 
        override
        {
            map.insert( { name, std::make_unique<RaytracerLayout>(std::move(*this)) } );
        }

    void SetRaytracer(Raytracer* raytracer) {
        raytracer_ = raytracer;
    }

    void SetBuffer(RaytracerPrimitive type, void* data)
    {
        buffer_ = std::make_unique<RaytracerBuffer>(type, data);
    }

    RaytracerBuffer& Buffer(){
        return *buffer_;
    }

};

}
