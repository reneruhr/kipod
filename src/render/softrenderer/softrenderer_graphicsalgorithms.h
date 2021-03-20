#pragma once
#include "../../core.h"


namespace kipod{
class graphicsalgorithms
{
public:
    graphicsalgorithms();
};

typedef unsigned int outcode;
enum {
    X0 = 1 << 0, //1
    X1 = 1 << 1, //2
    Y0 = 1 << 2, //4
    Y1 = 1 << 3, //8
    Z0 = 1 << 4, //16
    Z1 = 1 << 5 //32
};

inline std::string codeReader(outcode code){
    std::string s;
    switch(code){
        case(X0): s= " X=0";break;
        case(X1): s= " X=1";break;
        case(Y0): s= " Y=0";break;
        case(Y1): s= " Y=1";break;
        case(Z0): s= " Z=0";break;
        case(Z1): s= " Z=1";break;
    }
    return s;
    }

enum ClipInfo{
    CULLED,
    CULLED_NONTRIVIAL,
    KEEP,
    P,
    Q,
    PQ
};

struct Point{
    Point(const vec4& p): p(p){}
    vec4 p;
    float& operator[](const int i){
        assert(i<4);
        return p[i];
    }
};

using PointPtr = std::unique_ptr< Point >;

struct Line{
    Line(const vec4& p, const vec4& q): data{p,q}{}
    Line(const Line& line): data{line.data[0],line.data[1]}{}

    vec4 data[2];

    Line& operator=(const Line& line){
        data[0] = line.data[0];
        data[1] = line.data[1];
        return *this;
    }

    vec4& operator[](const int i){
        assert(i < 2);
        return data[i];
    }

    const vec4& operator[](const int i) const{
        assert(i < 2);
        return data[i];
    }

    vec4& p(){
        return data[0];
    }
    vec4& q(){
        return data[1];
    }

    PointPtr point(const int i){
       return std::make_unique<Point>( Point(data[i % 2]) );
    }

    friend std::ostream& operator << ( std::ostream& os, const Line& line ) {
    return os
            << std::endl <<"P: " << line[0]
            << std::endl <<"Q: " << line[1];
    }
};

using LinePtr = std::unique_ptr< Line >;

struct Triangle{

    Triangle(const vec4 _data[3]): data{ _data[0], _data[1], _data[2] } {
        color_data[0] = vec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             1);
        color_data[1] = vec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             1);
        color_data[2] = vec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             1);
    }

    Triangle(const vec4 _data[3], const vec4 _normal_data[3]):
        data{ _data[0], _data[1], _data[2] },
        normal_data{ _normal_data[0], _normal_data[1], _normal_data[2] }  {
        color_data[0] = vec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             1);
        color_data[1] = vec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             1);
        color_data[2] = vec4(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
                             1);
    }

    Triangle(const vec4 _data[3], const vec4 _normal_data[3], const vec4 _color_data[3]):
        data{_data[0],_data[1],_data[2]},
        normal_data{ _normal_data[0], _normal_data[1], _normal_data[2] },
        color_data{ _color_data[0], _color_data[1], _color_data[2] }  {}

    Triangle(const vec4& p, const  vec4& q, const  vec4& r) : data{p,q,r}{}

    Triangle(const Triangle& t) :  data{t[0],t[1],t[2]},
                                   normal_data{t.n(0),t.n(1),t.n(2)},
                                   color_data{t.c(0),t.c(1),t.c(2)} {}

    Triangle& operator=(const Triangle& t){
        for(int i = 0; i<3 ; i++)  data[i]=t[i];
        for(int i = 0; i<3 ; i++)  normal_data[i]=t.n(i);
        for(int i = 0; i<3 ; i++)  color_data[i]=t.c(i);
        return *this;
    }

    vec4 data[3];
    vec4 normal_data[3];
    vec4 color_data[3];

    void setNormals(const vec4 n[3]) {
        for(int i = 0; i<3; i++) normal_data[i]=n[i];
    }
    void setNormals(const vec4& n0,const vec4& n1, const vec4& n2 ) {
        normal_data[0]=n0; normal_data[1]=n1; normal_data[2]=n2;
    }

    vec4& n(const int& i) {
        return normal_data[i];
    }
    const vec4& n(const int& i) const {
        return normal_data[i];
    }


    void setColors(const vec4 c[3]) {
        for(int i = 0; i<3; i++) color_data[i]=c[i];
    }
    void setColors(const vec4& c0,const vec4& c1, const vec4& c2 ) {
        color_data[0]=c0; color_data[1]=c1; color_data[2]=c2;
    }

    vec4& c(const int& i) {
        return color_data[i];
    }
    const vec4& c(const int& i) const {
        return color_data[i];
    }


    vec4& operator[](const int i){
        assert(i < 3);
        return data[i];
    }
    const vec4& operator[](const int i) const{
        assert(i < 3);
        return data[i];
    }
    LinePtr line(const int i){
       return std::make_unique<Line>( Line(data[i % 3], data[(i+1) % 3]) );
    }
    LinePtr normals(const int i){
       //LOG_DEBUG("Called LinePtr for Normals from Triangles with Data {}", normal_data[0]);
       return std::make_unique<Line>( Line(normal_data[i % 3], normal_data[(i+1) % 3]) );
    }

    friend std::ostream& operator << ( std::ostream& os, const Triangle& triangle ) {
    return os
            << std::endl <<"P: " << dehomogenize(triangle[0])
            << std::endl <<"Q: " << dehomogenize(triangle[1])
            << std::endl <<"R: " << dehomogenize(triangle[2]) ;
    }

};
using TrianglePtr = std::unique_ptr< Triangle >;

using Triangles = std::unique_ptr< std::vector<TrianglePtr> >;

using pt = std::function<void(int, int)>;
using col_pt = std::function<void(int, int, float*)>;
using line_function = std::function<void(int*, int*)>;

void drawLine(int p[2], int q[2], pt drawPoint,line_function drawStraightLine);
void drawLineDDA(int* p, int* q, pt  drawPoint, line_function drawStraightLine);
void drawLineBresenheim(int* p, int* q, pt  drawPoint, line_function drawStraightLine);
void drawLineMidpoint(int* p, int* q, pt  drawPoint, line_function drawStraightLine);
void drawLineBresenheimWiki(int* p, int* q, pt  drawPoint, line_function drawStraightLine);

void drawTriangle(int* p, int* q, int* r,
              pt  drawPoint, line_function drawStraightLine);
void drawTriangle(int triangle[3][2],
              pt  drawPoint, line_function drawStraightLine);
void drawTriangle(int triangle[3][2], std::function<void(int*, int*, pt, line_function)>  drawLine,
                    pt drawPoint, line_function drawStraightLine);

void drawTriangleWithColor(int a[2], int b[2], int c[2], float color_a[3], float color_b[3], float color_c[3],
              col_pt  drawPointWithColor);

bool CohenSutherlandClipping(int* p, int* q, int rect[4]);

bool BlinnClipping(float* p, float* q);

struct ClipData {
    outcode c0,c1;
    ClipInfo info;
};
ClipData BlinnClipping(Line &line, Line* normals = nullptr);

Triangles clipTriangle(TrianglePtr triangle, bool normalMode = false);

Point intersectionLineTriangle(Triangle triangle, Line line);
bool intersectionLineTriangleMoellerTrumbore(Triangle triangle, Line line, Point point);

bool inFrame(int triangle[3][2], int height, int width);

void clipToScreen(float* in, int* out, int width, int height);

void  symmClipToSimpleClip(float *pt);
void  simpleClipToSymmClip(float *pt);

void zBufferAlgorithm(int* zBuffer, float* frameBuffer, Triangle triangle, int width, int height, int zPrec=65535);
void projectToScreen(const vec3& in, vec3& out, int width, int height, int zPrec);
void rasterize(vec3& in, int* out, int width, int height);
float edgeFunction(const vec2& edgeP, const vec2& edgeQ, const vec2& point);
bool inTriangle(const vec2 triangle[3], const vec2& point, vec3& coords);
void bbox2d(const vec2 triangle[3], vec4& bbox);

}
