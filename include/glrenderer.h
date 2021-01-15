#pragma once

#include <vector>
#include "utils/vec.h"
#include "utils/mat.h"
#include "GL/glew.h"
#include "renderer.h"
#include "texture.h"

#include <render_object.h>

class Light;
class MaterialStruct;
class Camera;

class PointSet;
class Shape;

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


enum class WindowType{
    Box,
    Octagon
};

struct QuasiCrystal{
    WindowType window;
    Shape* shape;
    QuasiCrystal(WindowType window = WindowType::Box): window(window){}
};

struct Shape2d{
};

class Lights{};

struct ModelData{
    GLuint vbo,vao,ebo,
           nvao, nvbo,
           col_vao, col_ebo, col_vbo,
           tex_vao_, tex_ebo_, tex_vbo_;
    unsigned int indices_size;
    bool hasNormals;
    bool hasTexture;
    Texture texture_;

    ModelData(){}
};

struct LatticeData{
    QuasiCrystal qc;
    GLuint vao, vbo;
    GLuint u_binding_point= 1, u_buffer, u_block_index;
    GLuint u_binding_point_window= 2, u_buffer_window, u_block_index_window;
    int size;
    float point_size_=5.0f;
    float point_size_window_ = 3.0f;
    float alpha_ = 0.001f;
    float z_decay_ = 0.1;
    float w_decay_ = 0.1;
    vec4 z_color_ = {1.0f, 0.0f, 0.00f, 0.3f};
    vec4 w_color_ = {0.0f, 1.0f, 0.00f, 0.3f};

    float depth_ = -0.7f;

    LatticeData(){}
};

struct ShapeData{
    GLuint vao_, vbo_;
    int size_;
    float depth_ = -0.1f;
    ShapeData(){}
};



struct LightGLGS{
    GLuint type;
    GLuint source;
    GLuint color;
    GLuint on;
};



using namespace std;
class GLRenderer : public Renderer
{
    unsigned int _width, _height;

    vector< shared_ptr<ModelData> > models;
    vector< shared_ptr<LatticeData> > lattices;
    vector< shared_ptr<ShapeData> > shapes_;
	unsigned int VBO, VAO, EBO, coord_VBO,coord_VAO;
	unsigned int vbo_size = 2;
	unsigned int vbo_total[2];
	unsigned int vao_total[2];
    GLuint program, program2, program3, programQuasi, programLights,
           programQuasiOctagon, programShapeOctagon, programQuasiOctagonWindow,
           programTex, programQuasiOctagonTexture;
	GLuint matrix, matrix2, matrix3, normal_length;

public:
    GLRenderer(unsigned int width=800, unsigned int height=600): Renderer(width, height){}
    ~GLRenderer(void){};
	void Init();
    shared_ptr<ModelData> loadTriangles(const std::vector<vec3>* vertices, const std::vector<unsigned int>* indices,
                                        const std::vector<vec3>* normals=nullptr, const std::vector<unsigned int>* nindices=nullptr);
    void drawTriangles(shared_ptr<ModelData> model);
    void drawNormals(shared_ptr<ModelData> model);
    void drawColoredTriangles(shared_ptr<ModelData> model);
	void SetProgram();
	void SetProgramWithNormals();
	void SetProgramWithNormals_from_faces();

    shared_ptr<LatticeData> loadPoints(PointSet* points);
    void drawPoints(shared_ptr<LatticeData> lattice);
    void drawPointsInWindow(shared_ptr<LatticeData> lattice);


	void useProgram(int i);

	void SetUniformMVP(mat4& mvp);
	void SetUniformMVP(mat4& mvp, int i);
	void SetUniformNormalLength(float x, int i);
	void SetUniformMVP_Normal(mat4 p, mat4 v, mat4 m);
	void SetUniformMVP_Normal_from_faces(mat4 p, mat4 v, mat4 m);

    bool switchWireframeMode = true;

    void SetProgram(QuasiCrystal quasi);
    void useProgram(QuasiCrystal quasi);
    void SetUniform(QuasiCrystal quasi, mat4& pv, mat4& m, shared_ptr<LatticeData> data);
    void SetUniform(QuasiCrystal quasi, mat4& pv, mat4& m, shared_ptr<LatticeData> data, Shape* shape);



    void SetProgram(Lights lights);
    void SetUniform(mat4 &m, mat4 &v, mat4 &p, vector<Light *> &lights, MaterialStruct &material, Camera *camera);
    void useProgram(Lights light);

    void SetProgramTex();
    void SetUniformTex(mat4 &m, mat4 &v, mat4 &p, vector<Light *> &lights, MaterialStruct &material, Camera *camera, Texture* texture);
    void useProgramTex();


    shared_ptr<ModelData> loadColoredTriangles(const std::vector<vec3> *vertices, const std::vector<unsigned int> *indices, const std::vector<vec3> *normals, const std::vector<unsigned int> *nindices);

    void initUniformBlock(shared_ptr<LatticeData> lattice_data);
    void setUniformBlock(shared_ptr<LatticeData> lattice_data, std::vector<float> &window_size);

    void setUniformBlock(shared_ptr<LatticeData> lattice_data, Shape* shape);



    void DrawShape(shared_ptr<ShapeData> shape);
    shared_ptr<ShapeData> LoadShape(vector<vec2> *vertices_vector_);
    void useProgram(Shape2d shape);
    void SetUniform(Shape2d shape, mat4 &m,  Shape* shape_qc);
    void useProgramWindow(QuasiCrystal quasi);
    void initUniformBlockWindow(shared_ptr<LatticeData> lattice_data);
    shared_ptr<ShapeData> UpdateShape(shared_ptr<ShapeData> shape, vector<vec2> *vertices_);
    void UpdatePoints(PointSet *points);




    shared_ptr<ModelData> LoadGLTriangles(const std::vector<GLTriangle> *triangles, const std::vector<unsigned int> *indices);
    void DrawGLTriangles(shared_ptr<ModelData> model);
    void SwapPrograms();
};



