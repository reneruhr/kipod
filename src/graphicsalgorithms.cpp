#include "graphicsalgorithms.h"
#include "core.h"
#include <memory>

graphicsalgorithms::graphicsalgorithms()
{

}

using pt = std::function<void(int, int)>;
using col_pt = std::function<void(int, int, float*)>;
using line_function = std::function<void(int*, int*)>;

void drawLineDDA(int* p, int* q, pt  drawPoint, line_function drawStraightLine){
    if(p[0] == q[0]) {
        drawStraightLine(p,q);
        return;
    }

    if(p[0] > q[0]) std::swap(p,q);

    float m =float(q[1]-p[1])/float(q[0]-p[0]);
    float y=p[1];
    for(int i = p[0]; i<=q[0]; i++){
        drawPoint(i,round(y));
        y+=m;
    }
}

void drawLineBresenheim(int* p, int* q,
                        pt  drawPoint, line_function drawStraightLine)
{
        //LOG_DEBUG("Call drawLineBresenheim");
    if(p[0] == q[0]) {
        drawStraightLine(p,q);
        return;
    }
    if(p[0] > q[0]) std::swap(p,q);

    int dx = q[0]-p[0], dy = q[1] - p[1];

    int d = dx;

    int e = 2*(dx-dy);

    if( 0 <= dy && 0 <=e) // m in [0,1]
    {
        int y = p[1];
        drawPoint(p[0],p[1]);
        for(int i = p[0]+1; i<=q[0]; i++){

            if(d<=0)
            {
                y++;
                d+=e;
            }else   d-=2*dy;
            drawPoint(i,y);
        }
        return;
    } else if(dy > dx) // m >1
    {
        //flip x and y
        std::swap(dx,dy);
        d = dx;
        e = -e;

        int x = p[0];
        drawPoint(x,p[1]);
        for(int i = p[1]+1; i<=q[1]; i++){
            if(d<=0)
            {
                x++;
                d+=e;
            }else   d-=2*dy;
            drawPoint(x,i);
        }
        return;
    }
    else if(dy < 0 && dx+dy >= 0) // m in [-1,0)
    {
        int e = 2*(dx+dy);
        int y = p[1];
        drawPoint(p[0],y);
        for(int i = p[0]+1; i<=q[0]; i++){
            if(d<0)
            {
                y--;
                d+=e;
            }else   d+=2*dy;
            drawPoint(i,y);
        }
        return;
    }else{
        //flip x and y
        std::swap(dx,dy);
        d = dx;
        int e = 2*(dx+dy);
        e = -e;

        int x = p[0];
        drawPoint(x,p[1]);
        for(int i = p[1]-1; i>=q[1]; i--){
            if(d<0)
            {
                x++;
                d+=e;
            }else   d-=2*dy;
            drawPoint(x,i);
        }
        return;
    }



}
void drawLineMidpoint(int* p, int* q,
                      pt  drawPoint, line_function drawStraightLine)
{
    if(p[0] == q[0] || p[1] == q[1]) {
        drawStraightLine(p,q);
        return;
    }

    if(p[0] > q[0]) std::swap(p,q);					// q lies now to the right of p

    float m = float(q[1]-p[1])/float(q[0]-p[0]);

    auto f = [&](float x, float y)
    {
        return (p[1]-q[1])*x+(q[0]-p[0])*y+p[0]*q[1]-q[0]*p[1];
    };


    if(m<=1 && m>=0)
    {
        int y = p[1];
        float d = f(p[0]+1,y+0.5);

        for( int x = p[0]; x<= q[0]; x++)
        {
            drawPoint(x,y);
            if(d<0){
                y++;
                d+=(p[1]-q[1])+(q[0]-p[0]);
            }else
                d+=(p[1]-q[1]);
        }
    }
    else if(m>1)
    {
        int x = p[0];
        float d = f(x+0.5,p[1]+1);
        for( int y = p[1]; y<= q[1]; y++)
        {
            drawPoint(x,y);
            if(d>0){
                x++;
                d+=(p[1]-q[1])+(q[0]-p[0]);
            }else
                d+=(q[0]-p[0]);
        }
    }
    else if(m >= -1)
    {
        int y = p[1];
        float d = f(p[0]+1,y-0.5);
        for( int x = p[0]; x<= q[0]; x++)
        {
            drawPoint(x,y);
            if(d>0){
                y--;
                d+=(p[1]-q[1])-(q[0]-p[0]);
            }else
                d+=(p[1]-q[1]);
        }
    }
    else if(m<-1)
    {
        int x = p[0];
        float d = f(x-0.5,p[1]+1);
        for( int y = p[1]; y>= q[1]; y--)
        {
            drawPoint(x,y);
            if(d<0){
                x++;
                d+=(p[1]-q[1])-(q[0]-p[0]);
            }else
                d-=(q[0]-p[0]);
        }
    }
}

void drawLineBresenheimWiki(int* p, int* q,
                        pt  drawPoint, line_function drawStraightLine)
{

    int x0 = p[0];
    int y0 = p[1];
    int x1 = q[0];
    int y1 = q[1];

    int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2;
    for(;;){
        drawPoint(x0,y0);
        e2 = 2*err;
        if (e2 >= dy) {
            if (x0 == x1) break;
            err += dy; x0 += sx;
        }
        if (e2 <= dx) {
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }
    }
}

void drawTriangle(int *p, int *q, int *r,
                  pt  drawPoint, line_function drawStraightLine)
{
    //LOG_DEBUG("Call drawTriangle");
    drawLineBresenheim(p,q,
                       drawPoint, drawStraightLine);
    drawLineBresenheim(q,r,
                       drawPoint, drawStraightLine);
    drawLineBresenheim(r,p,
                       drawPoint, drawStraightLine);
}

void drawTriangle(int triangle[3][2],
                    pt  drawPoint, line_function drawStraightLine)
{
    drawTriangle(triangle[0], triangle[1], triangle[2], drawPoint, drawStraightLine);
}

void drawTriangle(int triangle[3][2], std::function<void(
                                               int*, int*, pt, line_function
                                                        )>  drawLine,
                    pt  drawPoint, line_function drawStraightLine)
{
    drawLine(triangle[0], triangle[1], drawPoint, drawStraightLine);
    drawLine(triangle[1], triangle[2], drawPoint, drawStraightLine);
    drawLine(triangle[2], triangle[0], drawPoint, drawStraightLine);
}


 // Ineffective Midpoint version (non-incremental)
void drawLine(int p[2], int q[2],
              pt  drawPoint, line_function drawStraightLine) // Fundamentals of Computer Graphics p163
{
    if(p[0] == q[0] || p[1] == q[1]) {
        drawStraightLine(p,q);
        return;
    }

    if(p[0] > q[0]) std::swap(p,q);					// q lies now to the right of p

    float m = (q[1]-p[1])/(q[0]-p[0]);

    auto f = [&](float x, float y)
    {
        return (p[1]-q[1])*x+(q[0]-p[0])*y+p[0]*q[1]-q[0]*p[1];
    };

    if(m<=1 && m>=0)
    {
        int y = p[1];
        for( int x = p[0]; x<= q[0]; x++)
        {
            drawPoint(x,y);
            if(f(x+1,y+0.5)<0)	y++;
        }
    }
    else if(m>1)
    {
        int x = p[0];
        for( int y = p[1]; y<= q[1]; y++)
        {
            drawPoint(x,y);
            if(f(x+0.5,y+1)>0)	x++;
        }
    }
    else if(m >= -1)
    {
        int y = p[1];
        for( int x = p[0]; x<= q[0]; x++)
        {
            drawPoint(x,y);
            if(f(x+1,y-0.5)>0)	y--;
        }
    }
    else if(m<-1)
    {
        int x = p[0];
        for( int y = p[1]; y>= q[1]; y--)
        {
            drawPoint(x,y);
            if(f(x-0.5,y+1)<0)	x++;
        }
    }
}




void drawTriangleWithColor(int a[2], int b[2], int c[2], float color_a[3], float color_b[3], float color_c[3],
                                                                           col_pt  drawPointWithColor){
    int x_min = std::min(std::min(a[0],b[0]),c[0]);
    int x_max = std::max(std::max(a[0],b[0]),c[0]);
    int y_min = std::min(std::min(a[1],b[1]),c[1]);
    int y_max = std::max(std::max(a[1],b[1]),c[1]);

    auto f = [&](int* p,int* q, int x, int y)
    {
        return (p[1]-q[1])*x+(q[0]-p[0])*y+p[0]*q[1]-q[0]*p[1];
    };

    float alpha,beta,gamma;
    float falpha = f(b,c,a[0],a[1]);
    float fbeta = f(c,a,b[0],b[1]);
    float fgamma = f(a,b,c[0],c[1]);


    for(int y = y_min; y<=y_max; y++)
    {
        for(int x = x_min; x<=x_max; x++)
        {
            alpha = f(b,c,x,y)/falpha;
            beta = f(c,a,x,y)/fbeta;
            gamma = f(a,b,x,y)/fgamma;
            if(alpha > 0 && beta > 0 && gamma > 0){

                float color[3] = {
                                    alpha*color_a[0] + beta*color_b[0] + gamma * color_c[0],
                                    alpha*color_a[1] + beta*color_b[1] + gamma * color_c[1],
                                    alpha*color_a[2] + beta*color_b[2] + gamma * color_c[2],
                                };
                drawPointWithColor(x,y,color);
            }
        }
    }

}


// Computer Graphics Principles and Practice 2 Ed p. 116
//  Interactive Computer Graphics A Top-Down Approach with Shader-Based OpenGL p 311

// rect = l,r,b,t = xmin,xmax,ymin,ymax


enum{
    TOP = 0x0001,
    BOTTOM = 0x0002,
    RIGHT = 0x0004,
    LEFT = 0x0008
};

outcode produceOutcode(int p[2], int rect[4]){
    outcode outcodeOut = 0;

    if(p[0]<rect[0]) outcodeOut |= LEFT;
    else if(p[0]>rect[1]) outcodeOut |= RIGHT;
    if(p[1]<rect[2]) outcodeOut |= BOTTOM;
    else if(p[1]>rect[3]) outcodeOut |= TOP;

    return outcodeOut;
}


bool CohenSutherlandClipping(int* p, int* q, int rect[4])
{
    outcode outcode0 = produceOutcode(p, rect),
            outcode1 = produceOutcode(q, rect),
            outcodeOut;

    do
    {
        if(!(outcode0 | outcode1)){ //both values are 0 so both inside
            //LOG_DEBUG("Done Suther: True");
            return true;
        } else if(outcode0 & outcode1) //both outside on same side
           {
            //LOG_DEBUG("Done Suther: False. Line outside");
            return false;
        }
        else
        {

            // special cases if m=0/infty
            if(q[1]==p[1]){
                switch(outcode0){
                    case LEFT: {
                        p[0] = rect[0];
                        break;
                    }
                    case RIGHT: {
                        p[0] = rect[1];
                        break;
                    }
                }
                switch(outcode1){
                    case LEFT: {
                        q[0] = rect[0];
                        break;
                    }
                    case RIGHT: {
                        q[0] = rect[1];
                        break;
                    }
                }
                return true;
            }
            if(q[0]==p[0]){
                switch(outcode0){
                    case BOTTOM: {
                        p[1] = rect[2];
                        break;
                    }
                    case TOP: {
                        p[1] = rect[3];
                        break;
                    }
                }
                switch(outcode1){
                    case BOTTOM: {
                        q[1] = rect[2];
                        break;
                    }
                    case TOP: {
                        q[1] = rect[3];
                        break;
                    }
                }
                return true;
            }


            float x,y;
             outcodeOut = outcode0 ? outcode0 : outcode1;
             if(outcodeOut & TOP){ // use explicit line equation
                y = rect[3];
                x = p[0]+(q[0]-p[0])*(y-p[1])/(q[1]-p[1]);
             }else if(outcodeOut & BOTTOM){
                 y = rect[2];
                 x = p[0]+(q[0]-p[0])*(y-p[1])/(q[1]-p[1]);
             }else if(outcodeOut & RIGHT){
                 x = rect[1];
                 y = p[1]+(q[1]-p[1])*(x-p[0])/(q[0]-p[0]);
             }else{
                 x = rect[0];
                 y = p[1]+(q[1]-p[1])*(x-p[0])/(q[0]-p[0]);
             }

             if(outcodeOut == outcode0){
                 p[0] = round(x);
                 p[1] = round(y);
                 outcode0 = produceOutcode(p, rect);
             }else{
                 q[0] = round(x);
                 q[1] = round(y);
                 outcode1 = produceOutcode(q, rect);
             }
        }
    }while(1);
}

bool inFrame(int triangle[3][2], int height, int width){
    //LOG_DEBUG("Call inFrame");
    for(int i=0; i<3; i++){
        if(triangle[i][0] < 0 || triangle[i][0] >= width
                || triangle[i][1] < 0 || triangle[i][1] >= height)
            return false;
    }
    //LOG_DEBUG("Triangle inside ({},{}), ({},{}), ({},{})",triangle[0][0],triangle[0][1],triangle[1][0],triangle[1][1],triangle[2][0],triangle[2][1] );
    return true;
}



void produceBC(const vec4& p, std::vector<float>& BC){
   float x = p[0], y = p[1], z = p[2], w = p[3];
    BC[0] = x;
    BC[1] = w-x;
    BC[2] = y;
    BC[3] = w-y;
    BC[4] = z;
    BC[5] = w-z;
}
outcode produceOutcode(std::vector<float>& BC){
    outcode code = 0;
    const float EPSILON = -0.0000001;
    if(BC[0]<= EPSILON)             code |= X0;
    else if(BC[1] <= EPSILON)    code |= X1;
    if(BC[2]<EPSILON)             code |= Y0;
    else if(BC[3] <= EPSILON)    code |= Y1;
    if(BC[4]< EPSILON)             code |= Z0;
    else if(BC[5] <= EPSILON)    code |= Z1;

   return code;
}


// from [-1,1]^3 to [0,1]^3
void  symmClipToSimpleClip(vec4& pt){
    for(unsigned int i = 0; i<3; i++){
        pt[i]+= pt[3];
        pt[i]/= 2.0f;
    }
}
void  simpleClipToSymmClip(vec4& pt){
    for(unsigned int i = 0; i<3; i++){
        pt[i]*=2.0f;
        pt[i]-= pt[3];
    }
}


// float 4d point in homogeneous coordinates, assume [-1,1]^3 after w-divide
// sets int 2d point in [0,width]x[0,height]
void clipToScreen(float* in, int* out, int width, int height)
{
    float x = in[0], y = in[1], w = in[3];
    if( !(w == 0.0f) )  { x/=w; y/=w; }
    x+=1;
    x/=2;
    x*=width;
    y+=1;
    y/=2;
    y*=height;
    out[0] = std::min(  std::max((int)round(x),0), width -1 );
    out[1] = std::min(  std::max((int)round(y),0), height-1 );
}

//Wikipedia
bool intersectionLineTriangleMoellerTrumbore(Triangle inTriangle, Line line, PointPtr point)
{
        const float EPSILON = 0.0000001;
        vec3 vertex0 = dehomogenize(inTriangle[0]);
        vec3 vertex1 = dehomogenize(inTriangle[1]);
        vec3 vertex2 = dehomogenize(inTriangle[2]);
        vec3 rayOrigin = dehomogenize(line[0]);
        vec3 rayEnd =dehomogenize(line[1]);
        vec3 rayVector = rayEnd - rayOrigin;

        vec3 edge1, edge2, h, s, q;
        float a,f,u,v;

        edge1 = vertex1 - vertex0;
        edge2 = vertex2 - vertex0;
        h = cross(rayVector,edge2);
        a = dot(edge1, h);
        if (a > -EPSILON && a < EPSILON)
            return false;    // This ray is parallel to this triangle.
        f = 1.0/a;
        s = rayOrigin - vertex0;
        u = f * dot(s,h);
        if (u < 0.0 || u > 1.0)
            return false;
        q = cross(s, edge1);
        v = f * dot(rayVector,q);
        if (v < 0.0 || u + v > 1.0)
            return false;
        // At this stage we can compute t to find out where the intersection point is on the line.
        float t = f * dot(edge2,q);
        if (t > EPSILON) // ray intersection
        {
            point = std::make_unique<Point>( vec4(rayOrigin + rayVector * t, 1) );
            return true;
        }
        else // This means that there is a line intersection but not a ray intersection.
            return false;
}

outcode getSide(PointPtr q){
        Point p = *q;
        float EPSILON = std::abs(0.0000001*p[3]);
        if(p[0] < EPSILON && p[0] > -EPSILON){
            return X0;
        }
        if(p[0]-p[3] < EPSILON && p[0]-p[3] > -EPSILON){
            return X1;
        }
        if(p[1] < EPSILON && p[1] > -EPSILON){
            return Y0;
        }
        if(p[1]-p[3] < EPSILON && p[1]-p[3] > -EPSILON){
            return Y1;
        }
        if(p[2] < EPSILON && p[2] > -EPSILON){
            return Z0;
        }
        if(p[2]-p[3] < EPSILON && p[2]-p[3] > -EPSILON){
            return Z1;
        }
        return 0;
}

Triangles clipTriangle(TrianglePtr triangle, bool normalMode)
{
    //LOG_DEBUG("Call clip Triangle to Triangle {}", *triangle);

    LinePtr lines[3] = {triangle->line(0),
                        triangle->line(1),
                        triangle->line(2)};
    ClipData clipData[3];
    LinePtr normals[3];
    if(normalMode == false){
        for(int i = 0; i<3 ;i++) clipData[i]=BlinnClipping(*lines[i].get());
    }
    else{
        for(int i = 0; i<3 ;i++) normals[i]=triangle->normals(i);
        //LOG_DEBUG("Copy Normals: {}", *normals[0].get());
        for(int i = 0; i<3 ;i++) clipData[i]=BlinnClipping(*lines[i].get(), normals[i].get());
    }

    Triangles triangles = std::make_unique< std::vector<TrianglePtr> >( std::vector<TrianglePtr>() );
    if(clipData[0].c0 & clipData[1].c0 & clipData[2].c0) {
         //LOG_DEBUG("Culled Triangle.");
        return triangles;
    }

    if(clipData[0].info == KEEP && clipData[1].info == KEEP && clipData[2].info == KEEP){
        //LOG_DEBUG("Return triangle untouched.");
        triangles->push_back( std::move(triangle) );
        return triangles;
    }


    for(int i = 0; i<3; i++){
        outcode p = getSide(lines[i]->point(1));
        outcode q = getSide(lines[(i+1)%3]->point(0));

        if( (clipData[i].info == Q  || clipData[i].info == PQ )
         && (clipData[(i+1)%3].info == P  || clipData[(i+1)%3].info == PQ )  )
        {
                if( p == q){

                    //LOG_DEBUG("Corner of triangle intersects single side {} ( {} ). Triangulate:", codeReader(clipData[i].c1 & clipData[(i+1) %3].c0), codeReader(p));

                    TrianglePtr triangle0 = std::make_unique<Triangle>( Triangle((*triangle)[i], (*lines[i])[1], (*lines[(i+1) % 3])[0]) );
                    TrianglePtr triangle1 = std::make_unique<Triangle>( Triangle( (*lines[(i+1) % 3])[0], (*triangle)[(i+2) % 3], (*triangle)[i] ) );
                    if(normalMode){
                        // Naive normal and color copy
                        triangle0->setNormals(  triangle->n(i), (*normals[i])[1], (*normals[(i+1) % 3])[0]        );
                        //LOG_DEBUG("Set Normals in Triangle with data {}", *triangle0->normals(0).get());
                        triangle1->setNormals(  (*normals[(i+1) % 3])[0], triangle->n((i+2) % 3), triangle->n(i)  );

                        triangle0->setColors( triangle->c(i), triangle->c((i+1)%3), triangle->c((i+1)%3)        );
                        triangle1->setColors( triangle->c((i+1)%3), triangle->c((i+2)%3), triangle->c(i)  );
                    }

                    Triangles triangles0 = clipTriangle(std::move(triangle0), normalMode);
                    Triangles triangles1 = clipTriangle(std::move(triangle1), normalMode);
                    //LOG_DEBUG("New triangles from Line0:");
                    for(auto& v : *triangles0){
                        Triangle tt = *v;
                        //LOG_DEBUG("{}", tt );
                    }
                    //LOG_DEBUG("New triangles from Line1:");
                    for(auto& v : *triangles1){
                        Triangle tt = *v;
                        //LOG_DEBUG("{}", tt );
                        triangles0->push_back(std::move(v));
                    }
                    return triangles0;
                }
        }
    }

    triangles->push_back( std::move(triangle) );
    return triangles;
}

// Blinn. A Trip Down the Graphics Pipeline: Line Clipping
// if return = true, p,q point to modify points inside [0,1]^3
// p,q Homogeneous coordinates.
//NOTE: we first move points from [-1,1]^3 to [0,1]^3
ClipData BlinnClipping(Line& line, Line* normals)
{

    vec4 P0 = line[0];
    vec4 P1 = line[1];

    //LOG_DEBUG("BlinnClipping In:");
    //LOG_DEBUG("P {}", dehomogenize(P0) );
    //LOG_DEBUG("Q {}", dehomogenize(P1) );
    symmClipToSimpleClip(P0);
    symmClipToSimpleClip(P1);

    std::vector<float> BC0(6);
    std::vector<float> BC1(6);
    produceBC(P0, BC0);
    produceBC(P1, BC1);
    outcode c0 = produceOutcode(BC0);
    outcode c1 = produceOutcode(BC1);
    ClipData clipData = ClipData{c0,c1, CULLED};

    if(c0 & c1) { // both on same side
        //LOG_DEBUG("CULL TRIVIAL");
        return clipData;
    }
    if(!(c0 | c1)) {
        clipData.info = KEEP;
        //LOG_DEBUG("KEEP");
        return clipData; // all values are 0 hence both inside
    }
    outcode clip = ( c0 | c1 );
    float alpha0 = 0.0f, alpha1 = 1.0f, alpha;
    outcode mask = 1;

    for(unsigned int i = 0 ; i < 6; i++, mask <<=1 ){
        if(clip & mask){
            alpha = BC0[i] / (BC0[i] - BC1[i]);
            if(c0 & mask) alpha0 = std::max(alpha0,alpha);
            else alpha1 = std::min(alpha1,alpha);
            if(alpha1 < alpha0) {
                clipData.info = CULLED_NONTRIVIAL;
                //LOG_DEBUG("CULL NONTRIVIAL");
                return clipData;}
        }
    }

    vec4 P_tmp;
    if(c0) {
        P_tmp = P0 + alpha0*( P1-P0 );
        clipData.info = P;
        //LOG_DEBUG("Changed P");
    }
    if(c1) {
        P1 = P0 + alpha1*( P1-P0 );
        clipData.info = clipData.info == P ? PQ : Q;
        //LOG_DEBUG("Changed Q");
    }
    if(c0) { P0 = P_tmp; }

    simpleClipToSymmClip(P0);
    simpleClipToSymmClip(P1);

    line[0] = P0;
    line[1] = P1;

    if(normals != nullptr){
        //LOG_DEBUG("Normal BlinnClipping in: {}", normals->p());
        Line tmp(*normals);
        normals->p() = tmp[0] + alpha0*( tmp[1] - tmp[0]);
        normals->q() = tmp[0] + alpha1*( tmp[1] - tmp[0]);
        //LOG_DEBUG("Normal BlinnClipping Out: {}", normals->p());
    }

    //LOG_DEBUG("BlinnClipping Out:");
    //LOG_DEBUG("P {}", dehomogenize( line[0]) );
    //LOG_DEBUG("Q {}", dehomogenize( line[1]) );
    return clipData;
}

void projectToScreen(const vec3& in, vec3& out, int width, int height, int zPrec)
{
    out = in;
    for(int i =0; i<3;i++){
        out[i]+=1;
        out[i]/=2;
    }
    out[0]*=width;
    out[1]*=height;
    out[2]*=zPrec;
}

void rasterize(vec3& in, int* out, int width, int height)
{
    out[0] = std::min(  std::max((int)round(in.x),0), width -1 );
    out[1] = std::min(  std::max((int)round(in.y),0), height-1 );
    out[2] = (int)round(in.z);
}


float edgeFunction(const vec2 &edgeP, const vec2 &edgeQ, const vec2 &point){
    return
            0.5f*((edgeQ.x-edgeP.x)*(point.y-edgeP.y) - (edgeQ.y-edgeP.y)*(point.x-edgeP.x));
}

bool inTriangle(const vec2 triangle[3], const vec2 &point, vec3 &coords)
{
    for(int i = 0; i<3; i++)
        coords[i] = edgeFunction(triangle[i], triangle[(i+1) % 3], point);

    float total = coords[0]+coords[1]+coords[2];
    for(int i = 0; i<3; i++)    coords[i] /= total;

    return (coords[0] > 0) && (coords[1] > 0) && (coords[2] > 0);
}

//bbox (l,r,b,t)
void bbox2d(const vec3 triangle[3], vec4& bbox){
    bbox.x = triangle[0].x;
    bbox.y = triangle[0].x;
    bbox.z = triangle[0].y;
    bbox.w = triangle[0].y;
    for(int i = 1; i<3; i++){
        if(triangle[i].x < bbox.x) bbox.x = triangle[i].x;
        if(triangle[i].x > bbox.y) bbox.y = triangle[i].x;
        if(triangle[i].y < bbox.z) bbox.z = triangle[i].y;
        if(triangle[i].y > bbox.w) bbox.w = triangle[i].y;
    }
}


void zBufferAlgorithm(int *zBuffer, float *frameBuffer, Triangle triangle, int width, int height, int zPrec)
{
    vec3 T[3];
    vec2 T2d[3];
//    LOG_DEBUG("#################### NEW TRIANGLE ######################");
    for(int i=0; i<3; i++)
    {
        vec3 edgeIn = dehomogenize(triangle[i]);
        //vec3 edgeIn(triangle[i].x, triangle[i].y, triangle[i].z);
        projectToScreen(edgeIn, T[i], width, height, zPrec);
        T2d[i].x = T[i].x;
        T2d[i].y = T[i].y;
//        LOG_DEBUG("x/y Value: {}", T2d[i]);
//        LOG_DEBUG("z   Value: {}", T[i].z);
    }
//    LOG_DEBUG(" Dehomogenized Triangle: {}", triangle);

    vec3 coords;
    vec4 bbox;
    vec3 in;
    int out[3];
    bbox2d(T, bbox);
    auto clam = [](float x, int H){return std::min(std::max((int)round(x),0),H-1);};

    for(int i = clam(bbox.x, width); i<clam(bbox.y, width); i++){
        for(int j = clam(bbox.z, height); j<clam(bbox.w, height); j++){
            if(inTriangle(T2d, vec2(i,j), coords)){
                in = coords[0]*T[0] + coords[1]*T[1] + coords[2]*T[2];
                rasterize(in, out, width, height);
                if( zBuffer[i+width*j] > out[2] ) {
                    zBuffer[i+width*j] = out[2];
                vec4 outCol = coords[0]*triangle.c(0) + coords[1]*triangle.c(1) + coords[2]*triangle.c(2);
                for(int k = 0; k < 3; k++)   frameBuffer[(i+width*j)*3+k] = outCol[k];
                }
            }
        }
    }


}


