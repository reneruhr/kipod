#pragma once

#include "../../core.h"
namespace kipod::MeshModels{
void LoadFile(std::string fileName,
              std::vector<vec3>& vertices_vector, std::vector<unsigned int>& indices_vector,
              std::vector<vec3>& normals_vector, std::vector<unsigned int>& nindices_vector,
              std::vector<vec2>& texture_vector, std::vector<unsigned int>& tindices_vector);
void CalculateNormals(std::vector<vec3>& vertices_vector, std::vector<unsigned int>& indices_vector,
                      std::vector<vec3>& normals_vector, std::vector<unsigned int>& nindices_vector);
void ReduceVertices(std::vector<vec3>& vertices_vector, std::vector<unsigned int>& indices_vector);
}
