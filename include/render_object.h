#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <vector>
#include <string>

#include "render_buffer.h"
#include "render_shader.h"
#include "render_manager.h"

#include "texture.h"
#include "render_primitive.h"
#include "render_material.h"


namespace kipod{


class RenderObject
{
public:
    RenderObjectType type_;

    RenderMaterial* mat_;
    glm::mat4 local_transform_ = glm::mat4(1.0);
    glm::mat4 world_transform_ = glm::mat4(1.0);

    glm::mat4 Transform() const {
        return world_transform_*local_transform_;
    }

    virtual void Draw() = 0;
    virtual void Setup()= 0;
};


class GLObject : public RenderObject
{
public:
    GLObject() {
        LOG_ENGINE("Call: GLObject Constructor.");
    }

    ElementsBuffer* ebo_ = nullptr;
    VertexAttributeObject* vao_ = nullptr;
    VertexBuffer* vbo_ = nullptr;
    std::vector<FrameBuffer*> fras_;
    std::vector<Texture*> texs_;

    //Active:
    Shader* sha_ = nullptr;
    Texture* tex_ = nullptr;
    FrameBuffer* fra_ = nullptr;

    virtual void Draw() override;
    virtual void Setup() override;
};





}

#endif // RENDEROBJECT_H
