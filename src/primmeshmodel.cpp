#include "primmeshmodel.h"
#include <numeric>


PrimMeshModel::PrimMeshModel(Primitive primitive, int n){
    _world_transform = mat4(1.0);
    loadPrimitive(primitive, n);
}

void PrimMeshModel::loadPrimitive(Primitive primitive, int n){

    switch(primitive){
        case Cube:{
         LOG_INFO("Create a Cube");
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

                    vertices_vector = vector<vec3>(vertices,vertices+36);
                    indices_vector = vector<unsigned int>(indices, indices+36);

                    calculateNormals();

                break;
        } // cube
        case Tetrahedron:{
            LOG_INFO("Create a Tetrahedron");
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
//            unsigned int indices[12] = {
//                3,1,0,
//                3,2,1,
//                0,1,2,
//                2,3,0
//            };
            vertices_vector = vector<vec3>(vertices,vertices+12);
            indices_vector = vector<unsigned int>(indices, indices+12);

            calculateNormals();
            break;
        } //tetrahedron
    case Sphere:{
        // Interactive computer graphics p281
        LOG_INFO("Create a Sphere");
        vector<vec3> tetra = {{0.0, 0.0, 1.0},
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
            LOG_DEBUG("Call for n={} divide triangle on {} {} {}", n, a, b, c);
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

        indices_vector = vector<unsigned int>(vertices_vector.size());
        std::iota(std::begin(indices_vector), std::end(indices_vector), 0);


        //reduceVertices();
        nindices_vector = indices_vector;
        normals_vector = vertices_vector;
        break;
    } //sphere

    }//switch
    createBBox();
    centerModel();
}
