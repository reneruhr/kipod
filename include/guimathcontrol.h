#ifndef GUIMATHCONTROL_H
#define GUIMATHCONTROL_H
#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <Eigen/Geometry>
#include <Eigen/QR>

#include "utils/mat.h"
#include "utils/vec.h"
#include <vector>
#include <string>

#include "math/lattice.h"



using std::vector;
using std::string;

typedef vector<vec4> Vectors4;
typedef vector<mat4> Matrices4;
typedef vector<vec2> Vectors2;


enum sl2Embedding {
        TOP_LEFT = 0,
        OUTER = 1,
        BOTTOM_RIGHT = 2,
        TOP_RIGHT = 3,
        BOTTOM_LEFT = 4,
        XZ = 5,
        YZ = 6,
        YW = 7,
        NOT_DEFINED = -1
    };

struct EmbeddingDescription {
        sl2Embedding embed;
        int pos[4];
        std::string name;
        EmbeddingDescription(sl2Embedding em, int pos_in[], std::string name_in){
            embed = em;
            for(int i = 0; i < 4; ++i) pos[i] = pos_in[i];
            name = name_in;
        }
};

enum SL2type {
            DILATE,
            SHEAR_U,
            SHEAR_L,
            ROTATE
        };

typedef std::tuple<SL2type, sl2Embedding, float>  sl2insl4;

struct SampleSize
        {
            int x,y,z,w;
            SampleSize(int x,int y,int z,int w):x(x),y(y),z(z),w(w){}
        };






mat4 sl2tosl4(sl2insl4 sl2matrix);

class sl2{
    public:
        mat2 i = mat2(1.0f);

        mat2 dilate(float t);

        mat2 shear(float s);
        mat2 shearl(float s);

        mat2 rotate(float t);

        mat2 fromType(SL2type type, float t);
};
class sl4 : public sl2{
    public:
        mat4 i = mat4(1.0f);

        mat4 dilateTOPLEFT(float t);
        mat4 shearTOPLEFT(float t);
        mat4 shearlTOPLEFT(float t);
        mat4 rotateTOPLEFT(float t);

        Eigen::Vector4i outer = Eigen::Vector4i(0,3,2,1);
        Eigen::PermutationMatrix<4, 4> P = Eigen::PermutationMatrix<4, 4>(outer);

        mat4 dilateOuter(float t);
        mat4 shearOuter(float t);
        mat4 shearlOuter(float t);
        mat4 rotateOuter(float t);
};










class MatrixWalk{
    public:
        MatrixWalk(){}
        MatrixWalk(vector<sl2insl4> initMatrices){
            sl2stack=initMatrices;
            for(auto n: sl2stack) {
                sl4stack.push_back(sl2tosl4(n));
                all = sl4stack.back()*all;
            }
        }

        vector<sl2insl4> sl2stack = {};
        Matrices4 sl4stack = {};

        // Last element gets multiplied from the left.
        mat4 all = mat4(1.0f);


        void append(mat4 m);
        void append(mat4 m, sl2insl4 n);
        void append(sl2insl4 n);
        mat4 multiplyAll();

        void modify(int i, sl2insl4 n);

        bool allUptoDate = true;

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
    void matrix(float* f, int size);
    void SetMat4(mat4& m);
    void matrix4i(int* f);
    void mat4selectable(string* b, std::array<int, 16> &selected);
    void SetMat2(float* f);
    bool sl2matrix(float& s, SL2type sl2type, int id);
    void sl4matrix(mat4& m);
    void sl2control(mat4& temporaryMatrixView, MatrixWalk &SL4walk, sl2Embedding currEmbedding);
    void sl4control(MatrixWalk &SL4walk);
    void verify4i(SampleSize &sampleSize, mode &currentMode);
    void embeddings(std::array<int, 16> &selected, sl2Embedding &currEmbedding);
    void embeddingsView(std::array<int, 16> &selected);

    void drawHull(Vectors2 hull);
};

#endif // GUIMATHCONTROL_H
