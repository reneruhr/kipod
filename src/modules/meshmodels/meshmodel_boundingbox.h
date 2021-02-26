#pragma once

#include "../../core.h"

namespace kipod::MeshModels{

struct BoundingBoxData{
    glm::vec3 size_;
    glm::vec3 center_;
    glm::mat4 transform_;

    BoundingBoxData() = default;
    BoundingBoxData(const std::vector<vec3> &vertices);
    BoundingBoxData(BoundingBoxData&&) = default;
    BoundingBoxData& operator=(BoundingBoxData&&) = default;
};

}
