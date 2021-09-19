#pragma once

#include "../../core.h"
namespace kipod::MeshModels{
using Vec3 = glm::vec3;
using Vec2 = glm::vec2;
void LoadFile(std::filesystem::path path,
              std::shared_ptr<std::vector<Vec3>> vertices_vector,std::shared_ptr<std::vector<unsigned int>> indices_vector,
              std::shared_ptr<std::vector<Vec3>> normals_vector, std::shared_ptr<std::vector<unsigned int>> nindices_vector,
              std::shared_ptr<std::vector<Vec2>> texture_vector, std::shared_ptr<std::vector<unsigned int>> tindices_vector);

void CalculateNormals(std::shared_ptr<std::vector<Vec3>> vertices_vector,
                      std::shared_ptr<std::vector<unsigned int>> indices_vector,
                      std::shared_ptr<std::vector<Vec3>> normals_vector,
                      std::shared_ptr<std::vector<unsigned int>> nindices_vector);

void ReduceVertices(std::vector<Vec3>& vertices_vector, std::vector<unsigned int>& indices_vector);
}
