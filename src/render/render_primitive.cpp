#include "render_primitive.h"

void kipod::CreateTriangleVector(std::vector<GLTriangle>& triangles, std::vector<vec3>& vertices_vector, std::vector<unsigned int>& indices_vector,
                          std::vector<vec3>& normals_vector, std::vector<unsigned int>& nindices_vector,
                          std::vector<vec2>& texture_vector, std::vector<unsigned int>& tindices_vector)
{
    for(int i=0, n=indices_vector.size(); i<n; i+=3){
        triangles.emplace_back(
                    GLTriangle(GLVertex(
                                   vertices_vector[indices_vector[i]],
                                   normals_vector[nindices_vector[i]],
                                   texture_vector[tindices_vector[i]]),
                                GLVertex(
                                    vertices_vector[indices_vector[i+1]],
                                    normals_vector[nindices_vector[i+1]],
                                    texture_vector[tindices_vector[i+1]]),
                                GLVertex(
                                    vertices_vector[indices_vector[i+2]],
                                    normals_vector[nindices_vector[i+2]],
                                    texture_vector[tindices_vector[i+2]])
                                ));
    }
}
