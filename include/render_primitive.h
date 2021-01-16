#ifndef RENDERPRIMITIVE_H
#define RENDERPRIMITIVE_H

#include <glm/matrix.hpp>


enum RenderObjectType
{
    RENDER_WIREFRAME   = 1 << 0,
    RENDER_VERTEX      = 1 << 1,
    RENDER_NORMALS     = 1 << 2,
    RENDER_TEXTURE     = 1 << 3,
    RENDER_DEPTH       = 1 << 4,
    RENDER_Dim2        = 1 << 5
};

namespace Kipod
{

class RenderPrimitive
{
public:
    RenderPrimitive();
    int size_;
};


template <int>
struct GLVertex : virtual public RenderPrimitive{
    GLVertex() = default;
};


template<>
struct GLVertex<RENDER_VERTEX | RENDER_NORMALS | RENDER_TEXTURE>
        : virtual public RenderPrimitive{
    glm::vec3 position_;
    glm::vec3 normal_;
    glm::vec2 texture_;

    GLVertex() = default;
    GLVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture)
        : position_(position), normal_(normal), texture_(texture) {}
};

template<>
struct GLVertex<RENDER_VERTEX | RENDER_NORMALS >
        : virtual public RenderPrimitive{
    glm::vec3 position_;
    glm::vec3 normal_;

    GLVertex() = default;
    GLVertex(glm::vec3 position, glm::vec3 normal)
        : position_(position), normal_(normal){}
};

template<>
struct GLVertex<RENDER_VERTEX >
        : virtual public RenderPrimitive{
    glm::vec3 position_;

    GLVertex() = default;
    GLVertex(glm::vec3 position)
        : position_(position){}
};


template<typename Vertex>
struct GLTriangle : virtual public RenderPrimitive{
    Vertex vertices_[3];

    GLTriangle() = default;
    GLTriangle(Vertex v, Vertex w, Vertex u) {
        vertices_[0] = v;
        vertices_[1] = w;
        vertices_[2] = u;
    }
};

}
#endif // RENDERPRIMITIVE_H
