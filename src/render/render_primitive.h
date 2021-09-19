#pragma once
#include "../core.h"

namespace kipod{
using Vec3 = glm::vec3;
using Vec2 = glm::vec2;
struct GLVertex{
    Vec3 position_;
    Vec3 normal_;
    Vec2 texture_;

    GLVertex() = default;
    GLVertex(const Vec3& position,const  Vec3& normal,const Vec2& texture)
        : position_(position), normal_(normal), texture_(texture) {}
};

struct GLTriangle{
    GLVertex vertices_[3];

    GLTriangle() = default;
    GLTriangle(const GLVertex& v,const GLVertex& w,const GLVertex& u) {
        vertices_[0] = v;
        vertices_[1] = w;
        vertices_[2] = u;
    }
};

void CreateTriangleVector(std::vector<GLTriangle>& triangles,
                         std::vector<Vec3>& vertices_vector, std::vector<unsigned int>& indices_vector,
                         std::vector<Vec3>& normals_vector, std::vector<unsigned int>& nindices_vector,
                         std::vector<Vec2>& texture_vector, std::vector<unsigned int>& tindices_vector);

}
