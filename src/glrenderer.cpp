#include "../include/core.h"
#include "../include/scene.h"
#include "../include/meshmodel.h"
#include "../include/glrenderer.h"
#include "../include/initshader.h"
#include "../include/pointset.h"

#include <numeric>
#include "../include/utils/buffer_packing.h"

//shared_ptr<ModelData> GLRenderer::loadColoredTriangles(const std::vector<vec3>* vertices, const std::vector<unsigned int>* indices,
//                                                       const std::vector<vec3>* normals,  const std::vector<unsigned int>* nindices)
//{
//    LOG_ENGINE("Call: Colored Triangles Setup");

//    models.push_back(make_shared<ModelData>(ModelData()));
//    auto model = models.back();

//    model->indices_size = size(*indices);

//    SetupColoredTriangles(vertices,indices,normals,nindices);
//    return model;
//}


//shared_ptr<ModelData> GLRenderer::LoadGLTriangles(const std::vector<GLTriangle>* triangles, const std::vector<unsigned int>* indices)
//{
//    LOG_ENGINE("Call: Textured Triangles Setup");
//    models.push_back(make_shared<ModelData>(ModelData()));
//    auto model = models.back();
//    model->indices_size = indices->size();
//    model->hasTexture = true;


//    auto gl = new kipod::GLObject();
//    model->gl_objects_.insert({"Textured Triangles", gl});


//    gl->ebo_ = new kipod::ElementsBuffer((void*)indices->data(), indices->size(), indices->size()*sizeof(unsigned int));
//    gl->ebo_->Set();

//    gl->vao_ = new kipod::VertexAttributeObject;
//    gl->vao_->Set();

//    unsigned int buffersize = triangles->size()*sizeof(GLTriangle);
//    gl->vbo_ = new kipod::VertexBuffer(nullptr, buffersize);
//    gl->vbo_->Add(0, buffersize, (void*)triangles->data());

//    gl->vbo_->Bind();

//    kipod::Attribute* att_v = new kipod::Attribute(0,3,sizeof(GLVertex),0);
//    kipod::Attribute* att_n = new kipod::Attribute(1,3,sizeof(GLVertex), offsetof(GLVertex, normal_));
//    kipod::Attribute* att_t = new kipod::Attribute(2,2,sizeof(GLVertex), offsetof(GLVertex, texture_));

//    gl->vao_->Add(att_v);
//    gl->vao_->Add(att_n);
//    gl->vao_->Add(att_t);
//    gl->vao_->SetAttributes();

//    gl->Unbind();

//    return model;
//}

//void GLRenderer::DrawGLTriangles(shared_ptr<ModelData> model)
//{

//    auto gl = model->gl_objects_["Textured Triangles"];
//    gl->Draw();


////    glActiveTexture(GL_TEXTURE0);
////    model->texture_.Bind();

////    glBindVertexArray(model->tex_vao_);
////    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->tex_ebo_); // Not working without
////    glDrawElements(GL_TRIANGLES, model->indices_size, GL_UNSIGNED_INT, (void*)0);

////    glBindBuffer(GL_ARRAY_BUFFER, 0);
////    glBindVertexArray(0);
//}

//void GLRenderer::SetUniform(vector<Light*>& lights, Camera* camera, MeshModel* model, Scene *scene)
//{
//    scene->BindLightUniforms(lights);
//    scene->BindMatrixUniformsForMesh(*model, *camera);
//    scene->BindMaterialUniforms(*(model->mat_));
//}

//void GLRenderer::SetUniformNormal(MeshModel* model, Scene *scene)
//{
//    scene->BindNormalUniforms(model->normal_length);
//}

//void GLRenderer::SetUniformTex(vector<Light*>& lights, Camera* camera, MeshModel* model, Scene *scene)
//{
//    //glUniform1f(glGetUniformLocation(programTex, model->texture->name_.c_str()), 0);
//   scene->BindTextureUniforms(model->tex_);
//   SetUniform(lights, camera, model, scene);
//}


//void GLRenderer::drawColoredTriangles(shared_ptr<ModelData> model)
//{
//    auto gl = model->gl_objects_["Colored Triangles"];
//    gl->Draw();
//}

//void GLRenderer::Draw(kipod::GLObject *object)
//{
//    object->Draw();
//}

//void GLRenderer::Setup(kipod::GLObject *object)
//{
//    object->Setup();
//}



void GLRenderer::SetProgram(){
    program = InitShader( "shaders/_vshader.glsl", "shaders/_fshader.glsl" );
}

void GLRenderer::SetProgramWithNormals(){
    program2 = InitShader( "shaders/normals.vert.glsl", "shaders/normals.geom.glsl", "shaders/normals.frag.glsl");
}

void GLRenderer::SetProgramWithNormals_from_faces(){
    program3 = InitShader( "shaders/_vshaderNormals_from_faces.glsl", "shaders/_gshaderNormals_from_faces.glsl", "shaders/_fshader_from_faces.glsl");
}

void GLRenderer::SetProgram(QuasiCrystal quasi){

        programQuasi = InitShader( "shaders/points.vert.glsl", "shaders/points.frag.glsl" );
        programQuasiOctagon = InitShader( "shaders/inside_polygon.vert.glsl", "shaders/points.frag.glsl" );
        programShapeOctagon= InitShader( "shaders/shape.vert.glsl", "shaders/shape.frag.glsl" );
        programQuasiOctagonWindow = InitShader( "shaders/inside_polygon_window.vert.glsl", "shaders/points.frag.glsl" );

        programQuasiOctagonTexture = InitShader( "shaders/inside_polygon.vert.glsl", "shaders/renderToTexture.frag.glsl" );

}

void GLRenderer::SwapPrograms(){
    std::swap(programQuasiOctagon, programQuasiOctagonTexture);
}

void GLRenderer::SetProgram(Lights light){
    programLights = InitShader( "shaders/lights.vert.glsl", "shaders/lights.frag.glsl" );
}

void GLRenderer::useProgram(QuasiCrystal quasi)
{
    GLuint prog = programQuasi;
    if(quasi.window == WindowType::Octagon)
        prog = programQuasiOctagon;

    glUseProgram(prog);
}

void GLRenderer::useProgramWindow(QuasiCrystal quasi)
{
    GLuint prog = programQuasi;
    if(quasi.window == WindowType::Octagon)
        prog = programQuasiOctagonWindow;

    glUseProgram(prog);
}

void GLRenderer::useProgram(Shape2d shape)
{
    glUseProgram(programShapeOctagon);
}


void GLRenderer::useProgram(Lights light)
{
    glUseProgram(programLights);
}

void GLRenderer::SetProgramTex()
{
    programTex = InitShader( "shaders/textures.vert.glsl", "shaders/textures.frag.glsl" );
}



//void GLRenderer::SetUniformTex(mat4 &m, mat4 &v, mat4 &p, vector<Light *> &lights, MaterialStruct &material, Camera *camera, Texture* texture)
//{
//    glUniform1f(glGetUniformLocation(programTex, texture->name_.c_str()), 0);
//    texture->Bind();

//    auto p_via_glm = MakeGLM(p);
//    auto v_via_glm = MakeGLM(v);
//    auto m_via_glm = MakeGLM(m);

//    auto verts_glm =  m_via_glm*v_via_glm;
//    auto norms_glm = glm::transpose(glm::inverse(verts_glm));

//    mat4 verts = v*m;
//    mat4 norms = transpose(Inverse(v*m));
//    GLuint viewMatrix = glGetUniformLocation(programTex, "v");
//    GLuint mv = glGetUniformLocation(programTex, "mv");
//    GLuint mv_normal = glGetUniformLocation(programTex, "mv_normal");
//    GLuint projection = glGetUniformLocation(programTex, "projection");
////    glUniformMatrix4fv(mv, 1, GL_TRUE, &verts[0][0]);
////    glUniformMatrix4fv(mv_normal, 1, GL_TRUE, &norms[0][0]);
////glUniformMatrix4fv(viewMatrix, 1, GL_TRUE, &v[0][0]);
//  //    glUniformMatrix4fv(projection, 1, GL_TRUE, &p[0][0]);

//     glUniformMatrix4fv(mv, 1, GL_TRUE, glm::value_ptr(verts_glm));
//     glUniformMatrix4fv(mv_normal, 1, GL_TRUE, glm::value_ptr(norms_glm));
//     glUniformMatrix4fv(projection, 1, GL_TRUE, glm::value_ptr(p_via_glm));
//     glUniformMatrix4fv(viewMatrix, 1, GL_TRUE, glm::value_ptr(v_via_glm));





//    GLuint ambient = glGetUniformLocation(programTex, "material.ambient");
//    GLuint diffuse = glGetUniformLocation(programTex, "material.diffuse");
//    GLuint specular = glGetUniformLocation(programTex, "material.specular");
//    GLuint emission = glGetUniformLocation(programTex, "material.emission");
//    GLuint shininess = glGetUniformLocation(programTex, "material.shininess");



//    glUniform4fv(ambient, 1, &material.ambient[0]);
//    glUniform4fv(diffuse, 1, &material.diffuse[0]);
//    glUniform4fv(specular, 1, &material.specular[0]);
//    glUniform4fv(emission, 1, &material.emission[0]);
//    glUniform1fv(shininess, 1, &material.shininess);

//    GLuint cameraLocation = glGetUniformLocation(programTex, "cameraLocation");
//    vec4 camLocation = vec4(camera->getEye());
//    glUniform4fv(cameraLocation, 1, &camLocation[0]);


//    LightGLGS lightsGLGS[3];
//    for(int i = 0; i<3; ++i){
//        std::string lightsi = "Lights["+std::to_string(i)+"]";
//        std::string t = lightsi+".type";
//        std::string s = lightsi+".source";
//        std::string c = lightsi+".color";
//        std::string o = lightsi+".on";
//        const char* tc = t.c_str();
//        const char* sc = s.c_str();
//        const char* cc = c.c_str();
//        const char* oc = o.c_str();
//        lightsGLGS[i].type = glGetUniformLocation(programTex, tc);
//        lightsGLGS[i].source = glGetUniformLocation(programTex, sc);
//        lightsGLGS[i].color = glGetUniformLocation(programTex, cc);
//        lightsGLGS[i].on = glGetUniformLocation(programTex, oc);

//        int type = lights[i]->getType();
//        vec4 source = lights[i]->getSource();
//        vec4 color = lights[i]->getColor();
//        int on = lights[i]->turnedOn;

//        glUniform1i(lightsGLGS[i].type, type);
//        glUniform4fv(lightsGLGS[i].source, 1, &source[0]);
//        glUniform4fv(lightsGLGS[i].color, 1, &color[0]);
//        glUniform1i(lightsGLGS[i].on, on);
//    }
//}

void GLRenderer::useProgramTex()
{
    glUseProgram(programTex);
}

void GLRenderer::SetUniform(QuasiCrystal quasi, mat4& pv, mat4& m, shared_ptr<LatticeData> data){
    GLuint prog = programQuasi;
    if(quasi.window == WindowType::Octagon)
        prog = programQuasiOctagon;

    GLuint cam_matrix = glGetUniformLocation(prog, "pv");
    GLuint transform_matrix = glGetUniformLocation(prog, "transform");
    glUniformMatrix4fv(cam_matrix, 1, GL_TRUE, &pv[0][0]);
    glUniformMatrix4fv(transform_matrix, 1, GL_TRUE, &m[0][0]);

    GLuint shape = glGetUniformLocation(prog, "shape");
    glUniform2fv(shape, 8, &data->qc.shape->transformed_vertices_[0][0]);


    GLuint point_size = glGetUniformLocation(prog, "point_size");
    glUniform1f(point_size, data->point_size_);

    GLuint alpha = glGetUniformLocation(prog, "alpha");
    glUniform1f(alpha, data->alpha_);

    GLuint zdecay = glGetUniformLocation(prog, "zdecay");
    glUniform1f(zdecay, data->z_decay_);
    GLuint wdecay = glGetUniformLocation(prog, "wdecay");
    glUniform1f(wdecay, data->w_decay_);

    GLuint zColor = glGetUniformLocation(prog, "zColor");
    glUniform4fv(zColor, 1, &data->z_color_[0]);
    GLuint wColor = glGetUniformLocation(prog, "wColor");
    glUniform4fv(wColor, 1, &data->w_color_[0]);
}

void GLRenderer::SetUniform(QuasiCrystal quasi, mat4 &pv, mat4 &m, shared_ptr<LatticeData> lattice_data, Shape *shape)
{
    GLuint prog = programQuasiOctagonWindow;

    GLuint shape_matrix = glGetUniformLocation(prog, "shape_transform");
    glUniformMatrix4fv(shape_matrix, 1, GL_TRUE, &shape->GetWorldTransform()[0][0]);

    GLuint shape_vectors = glGetUniformLocation(prog, "shape");
    glUniform2fv(shape_vectors, 8, &lattice_data->qc.shape->transformed_vertices_[0][0]);

    GLuint cam_matrix = glGetUniformLocation(prog, "pv");
    GLuint transform_matrix = glGetUniformLocation(prog, "transform");
    glUniformMatrix4fv(cam_matrix, 1, GL_TRUE, &pv[0][0]);
    glUniformMatrix4fv(transform_matrix, 1, GL_TRUE, &m[0][0]);

    GLuint point_size = glGetUniformLocation(prog, "point_size");
    glUniform1f(point_size, lattice_data->point_size_window_);

    GLuint depth = glGetUniformLocation(prog, "depth");
    glUniform1f(depth, lattice_data->depth_);

    GLuint alpha = glGetUniformLocation(prog, "alpha");
    glUniform1f(alpha, 1.0f);

    vec4 color = {0.3f,0.3f,0.3f,1.0f};
    GLuint zColor = glGetUniformLocation(prog, "zColor");
    glUniform4fv(zColor, 1, &color[0]);
    GLuint wColor = glGetUniformLocation(prog, "wColor");
    glUniform4fv(wColor, 1, &color[0]);
}

void GLRenderer::initUniformBlock(shared_ptr<LatticeData> lattice_data)
{    
    GLuint prog = programQuasi;
    if(lattice_data->qc.window == WindowType::Octagon)
        prog = programQuasiOctagon;

    lattice_data->u_block_index = glGetUniformBlockIndex(prog, "WindowBlock");
    glUniformBlockBinding(prog, lattice_data->u_block_index, lattice_data->u_binding_point);

    glGenBuffers(1, &lattice_data->u_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, lattice_data->u_buffer);
}


//void GLRenderer::SetUniform(Shape2d shape, mat4& m, Shape* shape_qc)
//{
//    GLuint transform_matrix = glGetUniformLocation(programShapeOctagon, "transform");
//    glUniformMatrix4fv(transform_matrix, 1, GL_TRUE, &m[0][0]);
//    GLuint depth_shape = glGetUniformLocation(programShapeOctagon, "depth");
//    glUniform1f(depth_shape, shape_qc->shape_data_->depth_);

//}

void GLRenderer::setUniformBlock(shared_ptr<LatticeData> lattice_data, std::vector<float>& window_size)
{
    glBindBuffer(GL_UNIFORM_BUFFER, lattice_data->u_buffer);
    glBufferData(GL_UNIFORM_BUFFER, size(window_size)*sizeof(float), window_size.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, lattice_data->u_binding_point, lattice_data->u_buffer);
}




//void GLRenderer::SetUniform(mat4& m, mat4& v, mat4& p, vector<Light*>& lights, MaterialStruct& material, Camera* camera)
//{

//    mat4 verts = v*m;
//    mat4 norms = transpose(Inverse(v*m));
//    GLuint viewMatrix = glGetUniformLocation(programLights, "v");
//    GLuint mv = glGetUniformLocation(programLights, "mv");
//    GLuint mv_normal = glGetUniformLocation(programLights, "mv_normal");
//    GLuint projection = glGetUniformLocation(programLights, "projection");
//    glUniformMatrix4fv(mv, 1, GL_TRUE, &verts[0][0]);
//    glUniformMatrix4fv(mv_normal, 1, GL_TRUE, &norms[0][0]);
//    glUniformMatrix4fv(projection, 1, GL_TRUE, &p[0][0]);
//    glUniformMatrix4fv(viewMatrix, 1, GL_TRUE, &v[0][0]);


//    GLuint ambient = glGetUniformLocation(programLights, "material.ambient");
//    GLuint diffuse = glGetUniformLocation(programLights, "material.diffuse");
//    GLuint specular = glGetUniformLocation(programLights, "material.specular");
//    GLuint emission = glGetUniformLocation(programLights, "material.emission");
//    GLuint shininess = glGetUniformLocation(programLights, "material.shininess");

//    glUniform4fv(ambient, 1, &material.ambient[0]);
//    glUniform4fv(diffuse, 1, &material.diffuse[0]);
//    glUniform4fv(specular, 1, &material.specular[0]);
//    glUniform4fv(emission, 1, &material.emission[0]);
//    glUniform1fv(shininess, 1, &material.shininess);

//    GLuint cameraLocation = glGetUniformLocation(programLights, "cameraLocation");
//    vec4 camLocation = vec4(camera->getEye());
//    //glUniform4fv(cameraLocation, 1, &camLocation[0]);

//    auto camLocation_glm = MakeGLM(camLocation);
//    glUniform4fv(cameraLocation, 1, glm::value_ptr(camLocation_glm));



//    LightGLGS lightsGLGS[3];
//    for(int i = 0; i<3; ++i){
//        std::string lightsi = "Lights["+std::to_string(i)+"]";
//        std::string t = lightsi+".type";
//        std::string s = lightsi+".source";
//        std::string c = lightsi+".color";
//        std::string o = lightsi+".on";
//        const char* tc = t.c_str();
//        const char* sc = s.c_str();
//        const char* cc = c.c_str();
//        const char* oc = o.c_str();
//        lightsGLGS[i].type = glGetUniformLocation(programLights, tc);
//        lightsGLGS[i].source = glGetUniformLocation(programLights, sc);
//        lightsGLGS[i].color = glGetUniformLocation(programLights, cc);
//        lightsGLGS[i].on = glGetUniformLocation(programLights, oc);

//        int type = lights[i]->getType();
//        vec4 source = lights[i]->getSource();
//        vec4 color = lights[i]->getColor();
//        int on = lights[i]->turnedOn;


//        auto color_glm = MakeGLM(color);
//        glUniform4fv(lightsGLGS[i].color, 1, glm::value_ptr(color_glm));
//        auto source_glm = MakeGLM(source);
//        glUniform4fv(lightsGLGS[i].source, 1, glm::value_ptr(source_glm));

//        glUniform1i(lightsGLGS[i].type, type);
////        glUniform4fv(lightsGLGS[i].source, 1, &source[0]);
////        glUniform4fv(lightsGLGS[i].color, 1, &color[0]);
//        glUniform1i(lightsGLGS[i].on, on);
//    }
//}

shared_ptr<LatticeData> GLRenderer::loadPoints(PointSet* points)
{
    lattices.emplace_back(make_shared<LatticeData>());
    auto lattice = lattices.back();
    lattice->size = size(points->sample_);
    glGenVertexArrays(1, &lattice->vao);
    glGenBuffers(1, &lattice->vbo);

    glBindVertexArray(lattice->vao);
    glBindBuffer(GL_ARRAY_BUFFER, lattice->vbo);
    glBufferData(GL_ARRAY_BUFFER, lattice->size * 4 * sizeof(float), points->sample_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return lattice;
}

void GLRenderer::drawPoints(shared_ptr<LatticeData> lattice)
{
    glBindVertexArray(lattice->vao);
    glBindBuffer(GL_ARRAY_BUFFER, lattice->vbo);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_POINTS, 0, lattice->size);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GLRenderer::drawPointsInWindow(shared_ptr<LatticeData> lattice)
{
    drawPoints(lattice);
}

void GLRenderer::useProgram(int i)
{
    GLuint prog=program;
    if(i==2) prog=program2;
	else if(i==3) prog=program3;
	glUseProgram(prog);
}

void GLRenderer::SetUniformNormalLength(float x, int i)
{
    GLuint prog=program;
    if(i==2) prog=program2;
	else if(i==3) prog=program3;
    normal_length = glGetUniformLocation(prog, "normal_length");
    glUniform1f(normal_length, x);

}

void GLRenderer::SetUniformMVP(mat4& mvp)
{
    matrix = glGetUniformLocation(program, "mvp");
    glUniformMatrix4fv(matrix, 1, GL_TRUE, &mvp[0][0]);
}

void GLRenderer::SetUniformMVP(mat4& mvp, int i)
{
	GLuint prog;
	if(i==1) prog=program;
    else prog=program2;
    matrix = glGetUniformLocation(prog, "mvp");
    glUniformMatrix4fv(matrix, 1, GL_TRUE, &mvp[0][0]);
}

void GLRenderer::SetUniformMVP_Normal(mat4 p, mat4 v, mat4 m)
{
	mat4 verts = v*m;
	mat4 norms = transpose(Inverse(v*m));
    matrix = glGetUniformLocation(program2, "mv");
    matrix2 = glGetUniformLocation(program2, "mv_normal");
    matrix3 = glGetUniformLocation(program2, "projection");
    glUniformMatrix4fv(matrix, 1, GL_TRUE, &verts[0][0]);
    glUniformMatrix4fv(matrix2, 1, GL_TRUE, &norms[0][0]);
    glUniformMatrix4fv(matrix3, 1, GL_TRUE, &p[0][0]);
}

void GLRenderer::SetUniformMVP_Normal_from_faces(mat4 p, mat4 v, mat4 m)
{
	mat4 verts = v*m;
	mat4 norms = transpose(Inverse(v*m));
    matrix = glGetUniformLocation(program3, "mv");
    matrix2 = glGetUniformLocation(program3, "mv_normal");
    matrix3 = glGetUniformLocation(program3, "projection");
    glUniformMatrix4fv(matrix, 1, GL_TRUE, &verts[0][0]);
    glUniformMatrix4fv(matrix2, 1, GL_TRUE, &norms[0][0]);
    glUniformMatrix4fv(matrix3, 1, GL_TRUE, &p[0][0]);
}

void GLRenderer::drawTriangles(shared_ptr<ModelData> model)
{
    glBindVertexArray(model->vao);

    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
    glDrawElements(GL_TRIANGLES, model->indices_size, GL_UNSIGNED_INT, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void GLRenderer::drawNormals(shared_ptr<ModelData> model)
{
    if(model->hasNormals == false) return;
    glBindVertexArray(model->nvao);
    glBindBuffer(GL_ARRAY_BUFFER, model->nvbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_POINTS, 0, model->indices_size);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}




shared_ptr<ModelData> GLRenderer::loadTriangles(const std::vector<vec3>* vertices, const std::vector<unsigned int>* indices,
                                                const std::vector<vec3>* normals, const std::vector<unsigned int>* nindices){
    LOG_DEBUG("LoadTriangles Begin");

    models.push_back(make_shared<ModelData>(ModelData()));
    auto model = models.back();
    model->indices_size = indices->size();
    if(normals==nullptr) model->hasNormals = false;

    glGenVertexArrays(1, &model->vao);
    glGenBuffers(1, &model->vbo);
    glGenBuffers(1, &model->ebo);

    glBindVertexArray(model->vao);
    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    glBufferData(GL_ARRAY_BUFFER, 3*vertices->size()*sizeof(float), vertices->data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indices_size*sizeof(unsigned int), indices->data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if(!(normals==nullptr || nindices==nullptr)){
        LOG_DEBUG("Initializing normals");

        vector<vec3> vertices_and_normals_vector_gShader;
        for(unsigned int i = 0; i<model->indices_size; i++)
        {
            vertices_and_normals_vector_gShader.push_back((*vertices)[(*indices)[i]]);
            vertices_and_normals_vector_gShader.push_back((*normals)[(*nindices)[i]]);
        }

        glGenVertexArrays(1, &model->nvao);
        glGenBuffers(1, &model->nvbo);

        glBindVertexArray(model->nvao);
        glBindBuffer(GL_ARRAY_BUFFER, model->nvbo);
        glBufferData(GL_ARRAY_BUFFER, model->indices_size*2*3*sizeof(float), vertices_and_normals_vector_gShader.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    LOG_DEBUG("LoadTriangles End");
    return model;
}




shared_ptr<ShapeData> GLRenderer::LoadShape(vector<vec2>* vertices_)
{
    shapes_.emplace_back(make_shared<ShapeData>());
    auto shape = shapes_.back();
    shape->size_ = size(*vertices_);
    glGenVertexArrays(1, &shape->vao_);
    glGenBuffers(1, &shape->vbo_);

    glBindVertexArray(shape->vao_);
    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo_);
    glBufferData(GL_ARRAY_BUFFER, shape->size_ * sizeof(vec2), vertices_->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return shape;
}

void GLRenderer::UpdatePoints(PointSet* points)
{
    shared_ptr<LatticeData> lattice = points->lattice_data_;
    lattice->size = size(points->sample_);

    glBindVertexArray(lattice->vao);
    glBindBuffer(GL_ARRAY_BUFFER, lattice->vbo);
    glBufferData(GL_ARRAY_BUFFER, lattice->size * 4 * sizeof(float), points->sample_.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}


shared_ptr<ShapeData> GLRenderer::UpdateShape(shared_ptr<ShapeData> shape, vector<vec2>* vertices_)
{
    glBindVertexArray(shape->vao_);
    glBindBuffer(GL_ARRAY_BUFFER, shape->vbo_);
    glBufferData(GL_ARRAY_BUFFER, shape->size_ * sizeof(vec2), vertices_->data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return shape;
}


void GLRenderer::DrawShape(shared_ptr<ShapeData> shapeData)
{
    glBindVertexArray(shapeData->vao_);
    glBindBuffer(GL_ARRAY_BUFFER, shapeData->vbo_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, shapeData->size_);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}



