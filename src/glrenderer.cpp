#include "../include/core.h"
#include "../include/scene.h"
#include "../include/meshmodel.h"
#include "../include/glrenderer.h"
#include "../include/initshader.h"
#include "../include/pointset.h"

void GLRenderer::SetProgram(){
    program = InitShader( "shaders/_vshader.glsl", "shaders/_fshader.glsl" );
}

void GLRenderer::SetProgramWithNormals(){
    program2 = InitShader( "shaders/_vshaderNormals.glsl", "shaders/_gshaderNormals.glsl", "shaders/_fshader.glsl");
}

void GLRenderer::SetProgramWithNormals_from_faces(){
    program3 = InitShader( "shaders/_vshaderNormals_from_faces.glsl", "shaders/_gshaderNormals_from_faces.glsl", "shaders/_fshader_from_faces.glsl");
}


void GLRenderer::SetProgram(QuasiCrystal quasi){
    programQuasi = InitShader( "shaders/points.vert.glsl", "shaders/points.frag.glsl" );
}

void GLRenderer::useProgram(QuasiCrystal quasi)
{
        glUseProgram(programQuasi);
}

void GLRenderer::SetProgram(Lights light){
    programLights = InitShader( "shaders/lights.vert.glsl", "shaders/lights.frag.glsl" );
}

void GLRenderer::useProgram(Lights light)
{
        glUseProgram(programLights);
}

void GLRenderer::SetUniform(QuasiCrystal quasi, mat4& pv, mat4& m){

    GLuint cam_matrix = glGetUniformLocation(programQuasi, "pv");
    GLuint transform_matrix = glGetUniformLocation(programQuasi, "transform");
    glUniformMatrix4fv(cam_matrix, 1, GL_TRUE, &pv[0][0]);
    glUniformMatrix4fv(transform_matrix, 1, GL_TRUE, &m[0][0]);
}

void GLRenderer::initUniformBlock(shared_ptr<LatticeData> lattice_data)
{

    lattice_data->u_block_index = glGetUniformBlockIndex(programQuasi, "WindowBlock");
    glUniformBlockBinding(programQuasi, lattice_data->u_block_index, lattice_data->u_binding_point);

    glGenBuffers(1, &lattice_data->u_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, lattice_data->u_buffer);

}

void GLRenderer::setUniformBlock(shared_ptr<LatticeData> lattice_data, std::vector<float>& window_size)
{
    glBindBuffer(GL_UNIFORM_BUFFER, lattice_data->u_buffer);
    glBufferData(GL_UNIFORM_BUFFER, size(window_size)*sizeof(float), window_size.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, lattice_data->u_binding_point, lattice_data->u_buffer);
}

struct LightGLGS{
    GLuint type;
    GLuint source;
    GLuint color;
    GLuint on;
};

void GLRenderer::SetUniform(mat4& m, mat4& v, mat4& p, vector<Light*>& lights, MaterialStruct& material, Camera* camera)
{ 

    mat4 verts = v*m;
    mat4 norms = transpose(Inverse(v*m));
    GLuint viewMatrix = glGetUniformLocation(programLights, "v");
    GLuint mv = glGetUniformLocation(programLights, "mv");
    GLuint mv_normal = glGetUniformLocation(programLights, "mv_normal");
    GLuint projection = glGetUniformLocation(programLights, "projection");
    glUniformMatrix4fv(mv, 1, GL_TRUE, &verts[0][0]);
    glUniformMatrix4fv(mv_normal, 1, GL_TRUE, &norms[0][0]);
    glUniformMatrix4fv(projection, 1, GL_TRUE, &p[0][0]);
    glUniformMatrix4fv(viewMatrix, 1, GL_TRUE, &v[0][0]);


    GLuint ambient = glGetUniformLocation(programLights, "material.ambient");
    GLuint diffuse = glGetUniformLocation(programLights, "material.diffuse");
    GLuint specular = glGetUniformLocation(programLights, "material.specular");
    GLuint emission = glGetUniformLocation(programLights, "material.emission");
    GLuint shininess = glGetUniformLocation(programLights, "material.shininess");

    glUniform4fv(ambient, 1, &material.ambient[0]);
    glUniform4fv(diffuse, 1, &material.diffuse[0]);
    glUniform4fv(specular, 1, &material.specular[0]);
    glUniform4fv(emission, 1, &material.emission[0]);
    glUniform1fv(shininess, 1, &material.shininess);

    GLuint cameraLocation = glGetUniformLocation(programLights, "cameraLocation");
    vec4 camLocation = vec4(camera->getEye());

    glUniform4fv(cameraLocation, 1, &camLocation[0]);

    LightGLGS lightsGLGS[3];
    for(int i = 0; i<3; ++i){
        std::string lightsi = "Lights["+std::to_string(i)+"]";
        std::string t = lightsi+".type";
        std::string s = lightsi+".source";
        std::string c = lightsi+".color";
        std::string o = lightsi+".on";
        const char* tc = t.c_str();
        const char* sc = s.c_str();
        const char* cc = c.c_str();
        const char* oc = o.c_str();
        lightsGLGS[i].type = glGetUniformLocation(programLights, tc);
        lightsGLGS[i].source = glGetUniformLocation(programLights, sc);
        lightsGLGS[i].color = glGetUniformLocation(programLights, cc);
        lightsGLGS[i].on = glGetUniformLocation(programLights, oc);

        int type = lights[i]->getType();
        vec4 source = lights[i]->getSource();
        vec4 color = lights[i]->getColor();
        int on = lights[i]->turnedOn;

        glUniform1i(lightsGLGS[i].type, type);
        glUniform4fv(lightsGLGS[i].source, 1, &source[0]);
        glUniform4fv(lightsGLGS[i].color, 1, &color[0]);
        glUniform1i(lightsGLGS[i].on, on);

    }
}


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

    glPointSize(5.0);

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

void GLRenderer::useProgram(int i){
	GLuint prog;
	if(i==1) prog=program;
	else if(i==2) prog=program2;
	else if(i==3) prog=program3;
	glUseProgram(prog);
}

void GLRenderer::SetUniformNormalLength(float x, int i){
	GLuint prog;
	if(i==1) prog=program;
	else if(i==2) prog=program2;
	else if(i==3) prog=program3;
    normal_length = glGetUniformLocation(prog, "normal_length");
    glUniform1f(normal_length, x);

}

void GLRenderer::SetUniformMVP(mat4& mvp){

    matrix = glGetUniformLocation(program, "mvp");
    glUniformMatrix4fv(matrix, 1, GL_TRUE, &mvp[0][0]);
}

void GLRenderer::SetUniformMVP(mat4& mvp, int i){
	GLuint prog;
	if(i==1) prog=program;
    else prog=program2;
    matrix = glGetUniformLocation(prog, "mvp");
    glUniformMatrix4fv(matrix, 1, GL_TRUE, &mvp[0][0]);
}

void GLRenderer::SetUniformMVP_Normal(mat4 p, mat4 v, mat4 m){
	mat4 verts = v*m;
	mat4 norms = transpose(Inverse(v*m));
    matrix = glGetUniformLocation(program2, "mv");
    matrix2 = glGetUniformLocation(program2, "mv_normal");
    matrix3 = glGetUniformLocation(program2, "projection");
    glUniformMatrix4fv(matrix, 1, GL_TRUE, &verts[0][0]);
    glUniformMatrix4fv(matrix2, 1, GL_TRUE, &norms[0][0]);
    glUniformMatrix4fv(matrix3, 1, GL_TRUE, &p[0][0]);
}

void GLRenderer::SetUniformMVP_Normal_from_faces(mat4 p, mat4 v, mat4 m){
	mat4 verts = v*m;
	mat4 norms = transpose(Inverse(v*m));
    matrix = glGetUniformLocation(program3, "mv");
    matrix2 = glGetUniformLocation(program3, "mv_normal");
    matrix3 = glGetUniformLocation(program3, "projection");
    glUniformMatrix4fv(matrix, 1, GL_TRUE, &verts[0][0]);
    glUniformMatrix4fv(matrix2, 1, GL_TRUE, &norms[0][0]);
    glUniformMatrix4fv(matrix3, 1, GL_TRUE, &p[0][0]);
}

void GLRenderer::drawTriangles(shared_ptr<ModelData> model){
    glBindVertexArray(model->vao);

    glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
    glDrawElements(GL_TRIANGLES, model->indices_size, GL_UNSIGNED_INT, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void GLRenderer::drawNormals(shared_ptr<ModelData> model){
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

void GLRenderer::drawColoredTriangles(shared_ptr<ModelData> model){
    glBindVertexArray(model->col_vao);

    glBindBuffer(GL_ARRAY_BUFFER, model->col_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(vec3), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2*sizeof(vec3), (void*)sizeof(vec3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->col_ebo);
    glDrawElements(GL_TRIANGLES, model->indices_size, GL_UNSIGNED_INT, (void*)0);

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


shared_ptr<ModelData> GLRenderer::loadColoredTriangles(const std::vector<vec3>* vertices, const std::vector<unsigned int>* indices,
                                                       const std::vector<vec3>* normals,  const std::vector<unsigned int>* nindices){
    LOG_DEBUG("LoadColoredTriangles Begin");

    models.push_back(make_shared<ModelData>(ModelData()));
    auto model = models.back();
    model->indices_size = indices->size();

    LOG_DEBUG("Pack normals");

    vector<vec3> vnVector;
    for(unsigned int i = 0; i<model->indices_size; i++)
    {
        vnVector.push_back((*vertices)[(*indices)[i]]);
        vnVector.push_back((*normals)[(*nindices)[i]]);
    }

    glGenBuffers(1, &model->col_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->col_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indices_size*sizeof(unsigned int), indices->data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &model->col_vao);
    glBindVertexArray(model->col_vao);

    glGenBuffers(1, &model->col_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model->col_vbo);
    glBufferData(GL_ARRAY_BUFFER, vnVector.size()*sizeof(vec3), vnVector.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(vec3), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2*sizeof(vec3), (void*)sizeof(vec3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    LOG_DEBUG("LoadColoredTriangles End");
    return model;
}
