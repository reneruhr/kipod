#include "softrenderer.h"
#include "softrenderer_light.h"
#include "softrenderer_graphicsalgorithms.h"
#include "softrenderer_buffer.h"
#include "softrenderer_layout.h"
namespace kipod{

using namespace std::placeholders;


SoftRenderer::SoftRenderer(int width, int height) :    
    framebuffer_(std::make_unique<SoftRendererFramebuffer>(width, height)),
    uniform_(std::make_unique<SoftRendererUniform>()),
    width_(width),height_(height)
{
    InitOpenGLRendering();
    CreateBuffers();

    drawPointFunction = std::bind(&SoftRendererFramebuffer::DrawPoint, framebuffer_.get(), _1, _2);
    drawStraightLineFunction = std::bind(&SoftRendererFramebuffer::DrawStraightLine, framebuffer_.get(), _1, _2);
    drawPointWithColorFunction = std::bind(&SoftRendererFramebuffer::DrawPointWithColor, framebuffer_.get(), _1, _2, _3);
}

void SoftRenderer::SetUniforms(RenderCamera *camera, mat4 transform)
{
    uniform_->camera_ = camera;
    uniform_->object_transform_ = transform;
}

void SoftRenderer::CreateBuffers()
{
    CreateOpenGLBuffer();
    m_zbuffer = new int[width_*height_];
    initialize_zBuffer();
}

void SoftRenderer::initialize_zBuffer(int zPrec)
{
    for(int i = 0; i < width_*height_; i++) m_zbuffer[i] = zPrec;
}

void SoftRenderer::ClearBuffer()
{
    framebuffer_->Clear();
    initialize_zBuffer();
}

void SoftRenderer::DrawTriangles(RenderObject* object,
                                 bool wireframeMode, bool clippingMode)
{
   auto buffer = static_cast<SoftRenderLayout*>(object->Layout("SoftLayout"))->Buffer();
   RenderCamera& camera = *uniform_->camera_;
   mat4 model = uniform_->object_transform_;
   mat4 transform = mat4(camera) * model;
   mat4 normal_transform = transpose(Inverse(model));
   vec4 triangle[3];
   vec4 triangle_normal_ends[3];
   int rasterized_triangle[3][2];
   int rasterized_normal_ends[3][2];

   for(unsigned int j = 0; j<buffer.Count(); j+=3)
   {
       if(buffer.HasNormals()){
           for(int i = 0; i<3 ; i++)
                    triangle[i]=transform * buffer.Vertex(j,i);

           auto ts = clipTriangle(std::make_unique<Triangle>( Triangle(triangle) ));
           for(auto& t : *ts)       {
               if(wireframeMode){
                   for(int i = 0; i<3 ; i++)
                        clipToScreen((*t)[i], rasterized_triangle[i], width_, height_);
                   drawTriangleCall(rasterized_triangle);
               } else
                    zBufferAlgorithm(m_zbuffer, framebuffer_->Data(), *t, width_, height_);
           }
       }else
       {
          for(int i = 0; i<3 ; i++){
                   triangle[i]=transform * buffer.Vertex(j,i);
                   triangle_normal_ends[i] = mat4(camera)* normal_transform * buffer.Normal(j,i) + triangle[i];
          }

          Triangles ts;
          if(clippingMode)
                ts = clipTriangle(std::make_unique<Triangle>(Triangle(triangle,triangle_normal_ends)), true);
          else{
              ts = std::make_unique<std::vector<TrianglePtr>>(std::vector<TrianglePtr>());
              ts->emplace_back(std::make_unique<Triangle>(Triangle(triangle,triangle_normal_ends)));
          }

          for(auto& t : *ts)       {
              for(int i = 0; i<3 ; i++){
                   clipToScreen((*t)[i], rasterized_triangle[i], width_, height_);
                   clipToScreen((t->normal_data)[i], rasterized_normal_ends[i], width_, height_);
              }
              if(wireframeMode)
                    drawTriangleCall(rasterized_triangle);
              else
                   zBufferAlgorithm(m_zbuffer, framebuffer_->Data(), *t, width_, height_);

              for(int i = 0; i<3 ; i++) drawLineCall(rasterized_triangle[i], rasterized_normal_ends[i]);
          }
        }
   }
}





void SoftRenderer::DrawColoredTriangles(RenderObject* object, 
                                        const LightContainer& lights,
                                        bool emissiveMode)
{
    auto buffer = static_cast<SoftRenderLayout*>(object->Layout("SoftLayout"))->Buffer();
    RenderCamera& camera = *uniform_->camera_;
    mat4 model = uniform_->object_transform_;
    mat4 transform = mat4(camera) * model;
    mat4 normal_transform = transpose(Inverse(model));
    mat4 camera_transform = mat4(camera.view_matrix_);

    auto material = object->mat_;

    for(unsigned int j = 0; j<buffer.Count(); j+=3){
        vec4 triangle[3];
        vec4 triangle_normals[3];
        vec4 triangleColors[3];

        for(int i = 0; i<3 ; i++){
                 triangle[i]=transform * buffer.Vertex(j,i);
                 triangle_normals[i] = normal_transform * buffer.Normal(j,i);

                     for(const auto& light : lights){
                         if(light->Type()==kipod::LightSource::AMBIENT)
                            triangleColors[i]+=colorAmbient( material->ambient_,
                                                             light->Color());
                         if(light->Type()==kipod::LightSource::DIFFUSE)
                             triangleColors[i]+=colorDiffuse(material->diffuse_,
                                                             triangle,
                                                             triangle_normals,
                                                             camera_transform,
                                                             light.get());
                         if(light->Type()==kipod::LightSource::SPECULAR)
                             triangleColors[i]+=colorSpecular(material->specular_,
                                                 material->shininess_,
                                                 vec4(camera.eye_,0),
                                                 triangle,
                                                 triangle_normals,
                                                 camera_transform,
                                                 light.get());
                     }
                 if(emissiveMode)
                     triangleColors[i]+= material->emission_;
        }
        auto ts = clipTriangle(std::make_unique<Triangle>(Triangle(triangle, triangle_normals, triangleColors)), true);
        for(auto& t : *ts)       {
            zBufferAlgorithm(m_zbuffer, framebuffer_->Data(), *t, width_, height_, 65535);
        }
    }
}





void SoftRenderer::drawLineCall(int* p, int* q)
{
    switch(lineAlgo){
                    case DDA:
                        drawLineDDA(p, q, drawPointFunction, drawStraightLineFunction);
                        break;
                    case BREZENHAM:
                        drawLineBresenheim(p, q, drawPointFunction, drawStraightLineFunction);
                        break;
                    case MIDPOINT_SLOW:
                        drawLineMidpoint(p, q, drawPointFunction, drawStraightLineFunction);
                        break;
                    case BREZENHAM_WIKI:
                        drawLineBresenheimWiki(p, q, drawPointFunction, drawStraightLineFunction);
                        break;
    }
}

void SoftRenderer::drawTriangleCall(int triangle[3][2])
{
    switch(lineAlgo){
                    case DDA:
                         drawTriangle(triangle, drawLineDDA, drawPointFunction, drawStraightLineFunction);
                        break;
                    case BREZENHAM:
                        drawTriangle(triangle, drawLineBresenheim, drawPointFunction, drawStraightLineFunction);
                        break;
                    case MIDPOINT_SLOW:
                        drawTriangle(triangle, drawLineMidpoint, drawPointFunction, drawStraightLineFunction);
                        break;
                    case BREZENHAM_WIKI:
                        drawTriangle(triangle, drawLineBresenheimWiki, drawPointFunction, drawStraightLineFunction);
                        break;
    }
}


void SoftRenderer::InitOpenGLRendering()
{
    glGenTextures(1, &gScreenTex);
    glGenVertexArrays(1, &gScreenVtc);
    GLuint buffer;
    glBindVertexArray(gScreenVtc);
    glGenBuffers(1, &buffer);
    const GLfloat vtc[]={
        -1, -1,
        1, -1,
        -1, 1,
        -1, 1,
        1, -1,
        1, 1
    };
    const GLfloat tex[]={
        0,0,
        1,0,
        0,1,
        0,1,
        1,0,
        1,1};
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

    program = InitShader( "shaders/softrenderer.vert.glsl", "shaders/softrenderer.frag.glsl" );
    glUseProgram( program );
    GLint  vPosition = glGetAttribLocation( program, "vPosition" );

    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vtc) );
    glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );
}

void SoftRenderer::CreateOpenGLBuffer()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gScreenTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width_, height_, 0, GL_RGB, GL_FLOAT, NULL);
    glViewport(0, 0, width_, height_);
}

void SoftRenderer::DrawToOpenGL()
{
    glUseProgram( program );

    glBindVertexArray(gScreenVtc);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gScreenTex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGB, GL_FLOAT, framebuffer_->Data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}





}
