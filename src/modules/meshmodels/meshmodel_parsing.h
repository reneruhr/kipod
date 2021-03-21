#pragma once

#include "../../core.h"
namespace kipod::MeshModels{
void LoadFile(std::filesystem::path path,
              std::shared_ptr<std::vector<vec3>> vertices_vector,std::shared_ptr<std::vector<unsigned int>> indices_vector,
              std::shared_ptr<std::vector<vec3>> normals_vector, std::shared_ptr<std::vector<unsigned int>> nindices_vector,
              std::shared_ptr<std::vector<vec2>> texture_vector, std::shared_ptr<std::vector<unsigned int>> tindices_vector);

void CalculateNormals(std::shared_ptr<std::vector<vec3>> vertices_vector,
                      std::shared_ptr<std::vector<unsigned int>> indices_vector,
                      std::shared_ptr<std::vector<vec3>> normals_vector,
                      std::shared_ptr<std::vector<unsigned int>> nindices_vector);

void ReduceVertices(std::vector<vec3>& vertices_vector, std::vector<unsigned int>& indices_vector);
}
