#pragma once
#include "../core.h"

struct GLVertex{
    vec3 position_;
    vec3 normal_;
    vec2 texture_;

    GLVertex() = default;
    GLVertex(vec3 position, vec3 normal, vec2 texture)
        : position_(position), normal_(normal), texture_(texture) {}
};

struct GLTriangle{
    GLVertex vertices_[3];

    GLTriangle() = default;
    GLTriangle(GLVertex v, GLVertex w, GLVertex u) {
        vertices_[0] = v;
        vertices_[1] = w;
        vertices_[2] = u;
    }
};
