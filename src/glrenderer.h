#pragma once

#include <vector>
#include "utils/vec.h"
#include "utils/mat.h"
#include "GL/glew.h"
#include "renderer.h"
#include "render/render_texture.h"

#include "render/render_object.h"
#include "render/render_camera.h"



class Light;
class MaterialStruct;
class Camera;
class Scene;
class MeshModel;

class PointSet;
class Shape;


enum class WindowType{
    Box,
    Octagon
};

struct ModelData{
    GLuint vbo,vao,ebo,
           nvao, nvbo,
           col_vao, col_ebo, col_vbo,
           tex_vao_, tex_ebo_, tex_vbo_;
    unsigned int indices_size;
    bool hasNormals;
    bool hasTexture;
    kipod::Texture texture_;
    //std::unordered_map<std::string, kipod::GLObject*> gl_objects_;

    ModelData(){}
};

struct QuasiCrystal{
    WindowType window;
    Shape* shape;
    QuasiCrystal(WindowType window = WindowType::Box): window(window){}
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



public:
    GLRenderer(unsigned int width=800, unsigned int height=600): Renderer(width, height){
    }
    ~GLRenderer(void){};

    void UpdatePoints(PointSet *points);
    shared_ptr<ShapeData> UpdateShape(shared_ptr<ShapeData> shape, vector<vec2> *vertices_);
    void drawTriangles(shared_ptr<ModelData> model);
    void SwapPrograms();

};



