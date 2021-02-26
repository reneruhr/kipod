#include "meshmodel_boundingbox.h"

namespace kipod::MeshModels{
BoundingBoxData::BoundingBoxData(const std::vector<vec3> &vertices){
        glm::vec3 min = vertices[0];
        glm::vec3 max = vertices[0];
        for (auto v : vertices){
            if (v[0] < min[0]) min[0] = v[0];
            if (v[0] > max[0]) max[0] = v[0];
            if (v[1] < min[1]) min[1] = v[1];
            if (v[1] > max[1]) max[1] = v[1];
            if (v[2] < min[2]) min[2] = v[2];
            if (v[2] > max[2]) max[2] = v[2];
        }
        size_ = (max-min)/2.0f;
        center_= (max+min)/2.0f;
        transform_ = glm::translate(glm::mat4(1.0f), center_)*glm::scale(glm::mat4(1.0f), size_);
    }
}
