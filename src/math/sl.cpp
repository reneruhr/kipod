#include "../include/math/sl.h"
//#include <algorithm>

sl::sl()
{

}


mat4 sl2tosl4(sl2insl4 sl2matrix){
    sl2 sl2;
    mat4 m ;
    auto [type, embed, val] = sl2matrix;
//    Eigen::Vector4i perm_vec = Eigen::Vector4i(0,1,2,3);

//          if(embed == TOP_LEFT) ;
//    else if (embed == OUTER) perm_vec = Eigen::Vector4i(0,3,2,1);
//    else if (embed == BOTTOM_RIGHT) perm_vec = Eigen::Vector4i(2,3,0,1);
//    else if (embed == XZ) perm_vec = Eigen::Vector4i(0,2,1,3);
//    else if (embed == YZ) perm_vec = Eigen::Vector4i(1,2,0,3);
//    else if (embed == YW) perm_vec = Eigen::Vector4i(1,3,2,0);


    if (embed == TOP_RIGHT) m.topRightCorner(sl2.fromType(type, val));
    else if (embed == BOTTOM_LEFT) m.bottomLeftCorner(sl2.fromType(type, val));
    else if (embed == BOTTOM_RIGHT) m.bottomRightCorner(sl2.fromType(type, val));
    else if (embed == TOP_LEFT) m.topLeftCorner(sl2.fromType(type, val));

            //    else {
//        m.topLeftCorner<2,2>() = sl2.fromType(type, val);
//        Eigen::PermutationMatrix<4, 4> P = Eigen::PermutationMatrix<4, 4>(perm_vec);
//        m = P*m*P.transpose();
//    }
    return m;
}


mat2 sl2::fromType(SL2type type, float t){
        switch(type){
                case(DILATE):
                    return dilate(t);
                case(SHEAR_U):
                    return shear(t);
                case(SHEAR_L):
                    return shearl(t);
                case(ROTATE):
                    return rotate(t);
                default:
                    return i;
            }
}



mat2 sl2::dilate(float t){
    mat2 j = i;
    j[0][0] = exp(t);
    j[1][1] = exp(-t);
    return  j;
}

mat2 sl2::shear(float s){
    mat2 j = i;
    j[0][1] = s;
    return  j;
}
mat2 sl2::shearl(float s){
    mat2 j = i;
    j[1][0] = s;
    return  j;
}

mat2 sl2::rotate(float t){
    float angle = t*M_PI;
    mat2 j = mat2(vec2(cos(angle), -sin(angle)),
                  vec2(sin(angle), cos(angle)));
    return  j;
}

mat4 sl4::dilateTOPLEFT(float t){
    mat4 m;
    m.topLeftCorner(dilate(t));
    return m;
}
mat4 sl4::shearTOPLEFT(float t){
    mat4 m;
    m.topLeftCorner(shear(t));
    return m;
}
mat4 sl4::shearlTOPLEFT(float t){
    mat4 m;
    m.topLeftCorner(shearl(t));
    return m;
}
mat4 sl4::rotateTOPLEFT(float t){
    mat4 m;
    m.topLeftCorner(rotate(t));
    return m;
}

//mat4 sl4::dilateOuter(float t){
//    mat4 m = dilateTOPLEFT(t);
//    m = P.transpose()*m*P;
//    return m;
//}
//mat4 sl4::shearOuter(float t){
//    mat4 m = shearTOPLEFT(t);
//    m = P.transpose()*m*P;
//    return m;
//}
//mat4 sl4::shearlOuter(float t){
//    mat4 m = shearlTOPLEFT(t);
//    m = P.transpose()*m*P;
//    return m;
//}
//mat4 sl4::rotateOuter(float t){
//    mat4 m = rotateTOPLEFT(t);
//    m = P.transpose()*m*P;
//    return m;
//}










void MatrixWalk::append(sl2insl4 n){
    sl4stack.push_back(sl2tosl4(n));
    sl2stack.push_back(n);
    all = sl4stack.back()*all;

}
void MatrixWalk::append(mat4 m){
    sl4stack.push_back(m);
    sl2stack.push_back(std::make_tuple(DILATE, NOT_DEFINED, 0));
    all = m*all;
}
void MatrixWalk::append(mat4 m, sl2insl4 n){
    sl4stack.push_back(m);
    sl2stack.push_back(n);
    all = m*all;
}

mat4 MatrixWalk::multiplyAll(){
    if(allUptoDate == false){
        all = mat4(1.0f);
        for(auto& m: sl4stack) all = m*all;
        allUptoDate = true;
    }
    return all;
}

void MatrixWalk::modify(int i, sl2insl4 n){
            assert(i<sl4stack.size());
            sl4stack[i]=sl2tosl4(n);
            sl2stack[i]=n;
            allUptoDate = false;
}

