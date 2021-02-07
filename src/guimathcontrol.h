#ifndef GUIMATHCONTROL_H
#define GUIMATHCONTROL_H

#include "utils/mat.h"
#include "utils/vec.h"
#include <vector>
#include <string>

#include "math/lattice.h"
#include "math/sl.h"



using std::vector;
using std::string;

typedef vector<vec4> Vectors4;
typedef vector<mat4> Matrices4;
typedef vector<vec2> Vectors2;


struct SampleSize
        {
            int x,y,z,w;
            SampleSize(int x,int y,int z,int w):x(x),y(y),z(z),w(w){}
        };


enum mode {
       NEW_SAMPLE,
       IDLE
   };

//void basis4AsFloatAndStringArray(Lattice& L, float* f, string* b);

//void sl2AsFloatArray(mat2 local_sl2, float *sl2_print);

void embedding_rules(std::array<int, 16> &selected, sl2Embedding &currEmbedding);
void embedding_rules_reverse(std::array<int, 16> &selected, sl2Embedding &currEmbedding);


class GUIMathControl
{
public:
    GUIMathControl();

    sl2Embedding currEmbedding = NOT_DEFINED;
    vector<sl2insl4> SL2stack;
    MatrixWalk SL4walk;
    mat4 temporaryMatrixView;


    void matrix4(mat4& m);

    void sl2control(mat4& temporaryMatrixView, MatrixWalk &SL4walk, sl2Embedding currEmbedding);
    void sl4control(MatrixWalk &SL4walk);

    void embeddings(std::array<int, 16> &selected, sl2Embedding &currEmbedding);
    void embeddingsView(std::array<int, 16> &selected);


    void matrix(float* f, int size);
    void SetMat4(mat4& m);
    void matrix4i(int* f);
    void mat4selectable(string* b, std::array<int, 16> &selected);
    void SetMat2(float* f);
    bool sl2matrix(float& s, SL2type sl2type, int id);
    void sl4matrix(mat4& m);


    void verify4i(SampleSize &sampleSize, mode &currentMode);


    void drawHull(Vectors2 hull);
};

#endif // GUIMATHCONTROL_H
