#include "core.h"
#include "softrenderer.h"
#include "render/initshader.h"
#include "graphicsalgorithms.h"
#include <cmath>

using namespace std::placeholders;
#define INDEX(width,x,y,c) (x+y*width)*3+c

#include <iostream>


SoftRenderer::SoftRenderer() : Renderer()
{
    InitOpenGLRendering();
    CreateBuffers();
}

SoftRenderer::SoftRenderer(int width, int height) :
    Renderer(width, height)
{
    LOG_DEBUG("Called SoftRender Constructor");
    InitOpenGLRendering();
    CreateBuffers();
    drawPointFunction = std::bind(&SoftRenderer::drawPoint, *this, _1, _2);
    drawStraightLineFunction = std::bind(&SoftRenderer::drawStraightLine, *this, _1, _2);
    drawPointWithColorFunction = std::bind(&SoftRenderer::drawPointWithColor, *this, _1, _2, _3);
}

//SoftRenderer::~SoftRenderer(void)
//{
//}



void SoftRenderer::CreateBuffers()
{
    CreateOpenGLBuffer(); //Do not remove this line.
    m_outBuffer = new float[3*_width*_height];

    m_zbuffer = new int[_width*_height];
    initialize_zBuffer();
}

void SoftRenderer::initialize_zBuffer(int zPrec){
    for(int i = 0; i < _width*_height; i++) m_zbuffer[i] = zPrec;
}

void SoftRenderer::ClearBuffer()
{
    int s = 3*_width*_height;
    for(int i = 0; i< s; i++) m_outBuffer[i]=0;

    initialize_zBuffer();
}




void SoftRenderer::DrawTriangles(const std::vector<vec3>* vertices, const std::vector<unsigned int>* indices,
                                 bool wireframeMode, bool clippingMode,
                                 const std::vector<vec3>* normals, const std::vector<unsigned int>* nindices){
   //LOG_DEBUG("Call DrawTriangles");
   mat4 pc = _projection*_cTransform;
   mat4 transform = pc*_oTransform;
   //mat4 npc = _projection*transpose(Inverse(_cTransform));
   //LOG_DEBUG("Transform matrix: {}", transform);
   vec4 triangle[3];
   vec4 triangle_normal_ends[3];
   int rasterized_triangle[3][2];
   int rasterized_normal_ends[3][2];

   for(unsigned int j = 0; j<indices->size(); j+=3)
   {
       if(normals==nullptr){
           for(int i = 0; i<3 ; i++)
                    triangle[i]=transform * vec4((*vertices)[(*indices)[j+i]],1);

           auto ts = clipTriangle(std::make_unique<Triangle>( Triangle(triangle) ));
           for(auto& t : *ts)       {
               if(wireframeMode){
                   for(int i = 0; i<3 ; i++)
                        clipToScreen((*t)[i], rasterized_triangle[i], _width, _height);
                   drawTriangleCall(rasterized_triangle);
               } else
                    zBufferAlgorithm(m_zbuffer, m_outBuffer, *t, _width, _height);
           }
       }else
       {
          for(int i = 0; i<3 ; i++){
                   triangle[i]=transform * vec4((*vertices)[(*indices)[j+i]],1);
                   triangle_normal_ends[i] = pc*vec4(_nTransform*1.0f*(*normals)[(*nindices)[j+i]],0) + triangle[i];
                   //LOG_DEBUG("Normal Ends P{}", triangle_normal_ends[i]);
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
                   clipToScreen((*t)[i], rasterized_triangle[i], _width, _height);
                   //LOG_DEBUG("normal->data {}", t->normal_data[i]);
                   clipToScreen((t->normal_data)[i], rasterized_normal_ends[i], _width, _height);
              }
              if(wireframeMode)
                    drawTriangleCall(rasterized_triangle);
              else
                   zBufferAlgorithm(m_zbuffer, m_outBuffer, *t, _width, _height);

              for(int i = 0; i<3 ; i++) drawLineCall(rasterized_triangle[i], rasterized_normal_ends[i]);
          }
        }
   }
}




vec4 colorAmbient(const vec4& matColor, const vec4& lightColor){
    return matColor*lightColor;
}

vec4 colorDiffuse(const vec4& matColor,
                  vec4 triangle[3], vec4 normal_ends[3],
                  const mat4& transform,
                  Light* light){
    vec4 loc = (triangle[0]+triangle[1]+triangle[2])/3.0;
    vec4 l = -1.0f*transform * light->getSource()-loc;
    vec4 n = (normal_ends[0]+normal_ends[1]+normal_ends[2])/3.0;
    n.w = 0;
    n = normalize(n);
    vec4 lightColor = light->getColor();

    return matColor*lightColor*std::max(dot(l,n),0.0f);
}

vec4 colorSpecular(const vec4& matColor, const float& shininess, const vec4& camLocation,
                  vec4 triangle[3], vec4 normal_ends[3],
                  const mat4& transform,
                  Light* light){
    vec4 loc = (triangle[0]+triangle[1]+triangle[2])/3.0;
    vec4 l = -1.0f*transform* light->getSource()+loc;
    vec4 n = (normal_ends[0]+normal_ends[1]+normal_ends[2])/3.0;
    n.w = 0;
    n = normalize(n);
    vec4 lightColor = light->getColor();
    vec4 v = camLocation+loc;
    vec4 h = l+v;
    h.w =0;
    h = normalize(h);


    return matColor*lightColor*std::max(pow(dot(h,n), shininess), 0.0f);
}

void SoftRenderer::DrawColoredTriangles(const std::vector<vec3> *vertices, const std::vector<unsigned int> *indices,
                                        const std::vector<vec3> *normals, const std::vector<unsigned int> *nindices,
                                        const std::vector<MaterialStruct> *colors, const std::vector<unsigned int> *cindices,
                                        const std::vector<Light*> &lights,
                                        bool lightMode, bool emissiveMode)
{

    mat4 pc = _projection*_cTransform;
    mat4 transform = pc*_oTransform;

    for(unsigned int j = 0; j<indices->size(); j+=3){
        vec4 triangle[3];
        vec4 triangle_normals[3];
        vec4 triangleColors[3];

        for(int i = 0; i<3 ; i++){
                 triangle[i]=transform * vec4((*vertices)[(*indices)[j+i]],1);
                 triangle_normals[i] = vec4(_nTransform*1.0f*(*normals)[(*nindices)[j+i]],0);

                 if(lightMode){
                     for(const auto light : lights){
                         if(light->getType()==LightSource::AMBIENT)
                            triangleColors[i]+=colorAmbient( (*colors)[(*cindices)[j+i]].ambient,
                                                             light->getColor());
                         if(light->getType()==LightSource::DIFFUSE)
                             triangleColors[i]+=colorDiffuse((*colors)[(*cindices)[j+i]].diffuse,
                                                             triangle,
                                                             triangle_normals,
                                                             _cTransform,
                                                             light);
                         if(light->getType()==LightSource::SPECULAR)
                             triangleColors[i]+=colorSpecular((*colors)[(*cindices)[j+i]].specular,
                                                 (*colors)[(*cindices)[j+i]].shininess,
                                                 vec4(0,0,0,1), // CHANGED TO CAM
                                                 triangle,
                                                 triangle_normals,
                                                 _cTransform,
                                                 light);
                     }
                 }
                 if(emissiveMode)
                     triangleColors[i]+= (*colors)[(*cindices)[j+i]].emission;
        }
        auto ts = clipTriangle(std::make_unique<Triangle>(Triangle(triangle, triangle_normals, triangleColors)), true);
        for(auto& t : *ts)       {
            zBufferAlgorithm(m_zbuffer, m_outBuffer, *t, _width, _height, 65535);
        }
    }
}



void SoftRenderer::drawLineCall(int* p, int* q){
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




/////////////////////////////////////////////////////
//OpenGL stuff. Don't touch.

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

    program = InitShader( "shaders/softrenderer_vshader.glsl", "shaders/softrenderer_fshader.glsl" );
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
    glViewport(0, 0, _width, _height);
}

void SoftRenderer::SwapBuffers()
{
    glUseProgram( program );

    glBindVertexArray(gScreenVtc);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gScreenTex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGB, GL_FLOAT, m_outBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


void SoftRenderer::SetDemoBuffer()
{
    //LOG_DEBUG("Call SetDemoBuffer");
    //vertical line
    for(int i=0; i<_height; i++)
    {
        m_outBuffer[INDEX(_width,_width/2,i,0)]=1;	m_outBuffer[INDEX(_width,_width/2,i,1)]=0;	m_outBuffer[INDEX(_width,_width/2,i,2)]=0;
    }
    //horizontal line
    for(int i=0; i<_width; i++)
    {
        m_outBuffer[INDEX(_width,i,_height/2,0)]=1;	m_outBuffer[INDEX(_width,i,_height/2,1)]=0;	m_outBuffer[INDEX(_width,i,_height/2,2)]=1;

    }
}


void SoftRenderer::drawPoint(int x, int y)
{
    if( x < _width && y < _height && x>=0 && y>=0){
        m_outBuffer[INDEX(_width,x,y,0)]=1;
        m_outBuffer[INDEX(_width,x,y,1)]=1;
        m_outBuffer[INDEX(_width,x,y,2)]=1;
    }
}

void SoftRenderer::drawStraightLine(int *p, int *q)
{

    //LOG_DEBUG("Call drawStraightLine ({},{}) to ({}, {})",p[0],p[1],q[0],q[1]);
    if(p[0] == q[0] && p[0]<_width && p[0]>=0){ 								// p and q on same horizontal line
        if(p[1] > q[1]) std::swap(p,q);				// q lies now above p

        for( int i = std::max(p[1],0); i<= std::min(q[1],_height-1) ; i++)
        {
            m_outBuffer[INDEX(_width,p[0],i,0)]=1;
            m_outBuffer[INDEX(_width,p[0],i,1)]=1;
            m_outBuffer[INDEX(_width,p[0],i,2)]=1;
        }

    } else if(p[1] == q[1] && p[1]<_height && p[1]>=0){
        if(p[0] > q[0]) std::swap(p,q);
        for( int i = std::max(p[0],0); i<= std::min(q[0],_width-1); i++)
        {
            m_outBuffer[INDEX(_width,i,p[1],0)]=1;
            m_outBuffer[INDEX(_width,i,p[1],1)]=1;
            m_outBuffer[INDEX(_width,i,p[1],2)]=1;
        }
    }
}

void SoftRenderer::drawPointWithColor(int x, int y, float* c)
{
    m_outBuffer[INDEX(_width,x,y,0)]=*c;
    m_outBuffer[INDEX(_width,x,y,1)]=*(c+1);
    m_outBuffer[INDEX(_width,x,y,2)]=*(c+2);
}
