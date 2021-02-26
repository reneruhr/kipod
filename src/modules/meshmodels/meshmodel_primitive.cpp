#include "meshmodel_primitive.h"
#include "meshmodel_parsing.h"

namespace kipod::MeshModels{

PrimMeshModel::PrimMeshModel(Primitive primitive, int n){
    LoadPrimitive(primitive, n);
}

void PrimMeshModel::LoadPrimitive(Primitive primitive, int n){

    switch(primitive){
        case Cube:{
         LOG_ENGINE("Create a Cube");
         LOG_CONSOLE("Create a Cube");
         name_ = "Cube";

                    vec3 vertices[] = {
                                    {-1.0f,-1.0f,-1.0f},
                                    {-1.0f,-1.0f, 1.0f},
                                    {-1.0f, 1.0f, 1.0f}, //1
                                    {1.0f, 1.0f,-1.0f},
                                    {-1.0f,-1.0f,-1.0f},
                                    {-1.0f, 1.0f,-1.0f}, //2
                                    {1.0f,-1.0f, 1.0f},
                                    {-1.0f,-1.0f,-1.0f},
                                    {1.0f,-1.0f,-1.0f},//3
                                    {1.0f, 1.0f,-1.0f},
                                    {1.0f,-1.0f,-1.0f},
                                    {-1.0f,-1.0f,-1.0f},//4
                                    {-1.0f,-1.0f,-1.0f},
                                    {-1.0f, 1.0f, 1.0f},
                                    {-1.0f, 1.0f,-1.0f},//5
                                    {1.0f,-1.0f, 1.0f},
                                    {-1.0f,-1.0f, 1.0f},
                                    {-1.0f,-1.0f,-1.0f},//6
                                    {-1.0f, 1.0f, 1.0f},
                                    {-1.0f,-1.0f, 1.0f},
                                    {1.0f,-1.0f, 1.0f},//7
                                    {1.0f, 1.0f, 1.0f},
                                    {1.0f,-1.0f,-1.0f},
                                    {1.0f, 1.0f,-1.0f},//8
                                    {1.0f,-1.0f,-1.0f},
                                    {1.0f, 1.0f, 1.0f},
                                    {1.0f,-1.0f, 1.0f},//9
                                    {1.0f, 1.0f, 1.0f},
                                    {1.0f, 1.0f,-1.0f},
                                    {-1.0f, 1.0f,-1.0f},//10
                                    {1.0f, 1.0f, 1.0f},
                                    {-1.0f, 1.0f,-1.0f},
                                    {-1.0f, 1.0f, 1.0f},//11
                                    {1.0f, 1.0f, 1.0f},
                                    {-1.0f, 1.0f, 1.0f},
                                    {1.0f,-1.0f, 1.0f}//12
                                };
                    unsigned int *indices = new unsigned int[36];
                    for(unsigned int i = 0 ; i<36; i++) indices[i]=i;

                    vertices_vector = std::vector<vec3>(vertices,vertices+36);
                    indices_vector = std::vector<unsigned int>(indices, indices+36);

                    CalculateNormals(vertices_vector, indices_vector,
                                            normals_vector, nindices_vector);

                break;
        } // cube
        case Tetrahedron:{
            LOG_ENGINE("Create a Tetrahedron");
            LOG_CONSOLE("Create a Tetrahedron");
            name_ = "Tetrahedron";

            vec3 vertices[12] = {
                {0.0, 0.0, 1.0},
                {0.0, 0.942809, -0.333333},
                {-0.816497, -0.471405, -0.333333},
                {0.816497, -0.471405, -0.333333},
                {0.0, 0.0, 1.0},
                {0.0, 0.942809, -0.333333},
                {-0.816497, -0.471405, -0.333333},
                {0.816497, -0.471405, -0.333333},
                {0.0, 0.0, 1.0},
                {0.0, 0.942809, -0.333333},
                {-0.816497, -0.471405, -0.333333},
                {0.816497, -0.471405, -0.333333},
            };
            unsigned int indices[12] = {
                3,1,0,
                7,2,5,
                4,9,6,
                10,11,8
            };

            vertices_vector = std::vector<vec3>(vertices,vertices+12);
            indices_vector = std::vector<unsigned int>(indices, indices+12);

            CalculateNormals(vertices_vector, indices_vector,
                                    normals_vector, nindices_vector);
            break;
        } //tetrahedron
    case Sphere:{
        // Interactive computer graphics p281
        LOG_ENGINE("Create a Sphere");
        LOG_CONSOLE("Create a Sphere");
        name_ = "Sphere";
        std::vector<vec3> tetra = {{0.0, 0.0, 1.0},
                               {0.0, 0.942809, -0.333333},
                               {-0.816497, -0.471405, -0.333333},
                               {0.816497, -0.471405, -0.333333}
                               };

        auto triangle = [this](const vec3& a, const vec3& b, const vec3& c){
                                    vertices_vector.push_back(a);
                                    vertices_vector.push_back(b);
                                    vertices_vector.push_back(c);
                                };

        std::function<void(const vec3&,const vec3&,const vec3&,const int)> divide_triangle = [&divide_triangle,&triangle](const vec3& a, const vec3& b, const vec3& c, const int n){
            if(n>0){
                vec3 v1 = normalize(a+b);
                vec3 v2 = normalize(a+c);
                vec3 v3 = normalize(b+c);

                divide_triangle(v1,v3, v2, n-1);
                divide_triangle(a, v1, v2, n-1);
                divide_triangle(c, v2, v3, n-1);
                divide_triangle(b, v3, v1, n-1);


            }else
                triangle(a,b,c);
        };

        int k = n;
        divide_triangle(tetra[3], tetra[1], tetra[0], k);
        divide_triangle(tetra[3], tetra[2], tetra[1], k);
        divide_triangle(tetra[0], tetra[1], tetra[2], k);
        divide_triangle(tetra[2], tetra[3], tetra[0], k);

        indices_vector = std::vector<unsigned int>(vertices_vector.size());
        std::iota(std::begin(indices_vector), std::end(indices_vector), 0);


        //ReduceVertices();
        nindices_vector = indices_vector;
        normals_vector = vertices_vector;
        break;
    } //sphere

    }//switch
    CreateBoundingBox();
    CenterModel();
}

}