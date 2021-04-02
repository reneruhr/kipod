#define CATCH_CONFIG_MAIN
#include "../../external_libs/catch2/catch_amalgamated.hpp"

#include "../../siegel2021.h"

/*
####### Known Bugs #######

1. Unary- only for Matrix/Vector.
Cause: Unary not implemented for general Template Expression
2. return matrix*scalar and matrix/scalar not working
Cause: ?


*/
using namespace siegel2021;

TEST_CASE( "Create Matrix and use [][] for Assignment", "[Assignment]" ) {

    Matrix<int, 2> mat;
    mat[0][0] = 2;
    mat[1][1] = 3;

    REQUIRE( mat(0,0) == 2 );
    REQUIRE( mat(1,1) == 3 );
}

TEST_CASE( "Create Row Vector and use [] for Assignment", "[VectorAssignment]" ) {

    auto vec = Matrix<int, 2, 1>();
    vec[0] = 1;
    vec[1] = 2;

    REQUIRE( vec(0,0) == 1 );
    REQUIRE( vec(1,0) == 2 );
}

TEST_CASE( "Create Row Vector and use implicit Scalar cast", "[VectorScalarCast]" ) {

    auto vec = Matrix<int, 2, 1>({1,2});

    int v0= vec[0];

    REQUIRE( v0 == 1 );
    REQUIRE( vec[1] == 2 );
}

TEST_CASE( "Check if square", "[SquareMatrix]" ) {

    auto square = Matrix<float, 2, 2>();
    auto nonsquare = Matrix<float, 3, 2>();


    REQUIRE( IsSquare(square) );
    REQUIRE( not IsSquare(nonsquare) );
}

TEST_CASE( "Check if same size", "[SameSize]" ) {

    auto mat22 = Matrix<float, 2, 2>({1,2,3,4});
    auto mat22_2 = Matrix<float, 2, 2>({0,0,0,1});
    auto mat32 = Matrix<int, 3, 2>();


    REQUIRE( AreSameSize(mat22,mat22_2) );
    REQUIRE( not AreSameSize(mat22,mat32) );
}

TEST_CASE( "Columns and Rows", "[ColumnRows]" ) {

    Matrix<int, 2, 3> mat = { 1,2,3,4,5,6 };

    REQUIRE( Columns(mat)==3 );
    REQUIRE( Rows(mat)==2 );
}

TEST_CASE( "Transpose View", "[TransposeView]" ) {

    Matrix<int, 2, 3> mat = { 1,2,3,4,5,6 };
    auto transpose = ViewTranspose(mat);


    REQUIRE( Columns(transpose) == Rows(mat) );
    REQUIRE( (mat[0][0] == transpose(0,0) &&
              mat[0][1] == transpose(1,0)  &&
              mat[0][2] == transpose(2,0)) );
    REQUIRE( (mat[0][0] == ViewTranspose(mat)(0,0) &&
              mat[0][1] == ViewTranspose(mat)(1,0)  &&
              mat[0][2] == ViewTranspose(mat)(2,0)) );
}

TEST_CASE( "Transpose", "[Transpose]" ) {

    Matrix<int, 2, 3> mat = { 1,2,3,4,5,6 };
    auto transpose = Transpose(mat);


    REQUIRE( Columns(transpose) == Rows(mat) );
    REQUIRE( (mat[0][0] == transpose(0,0) &&
              mat[0][1] == transpose(1,0)  &&
              mat[0][2] == transpose(2,0)) );
}

TEST_CASE( "Addition", "[addition]" ) {

    auto vec = Matrix<int, 2, 1>();
    auto vec2 = Matrix<int, 2, 1>({1,1});

    decltype(vec) vec3 = vec+vec2;

    REQUIRE( vec3[0] == 1 );
    REQUIRE( vec3[1] == 1 );
}

TEST_CASE( "Addition Expression Template", "[additionExpression]" ) {

    auto vec = Matrix<int, 2, 1>();
    auto vec2 = Matrix<int, 2, 1>({1,1});


    REQUIRE( (vec+vec2)(0,0) == 1 );
    REQUIRE( (vec+vec2)(1,0) == 1 );
}

TEST_CASE( "Multiplication", "[multiplication]" ) {

    auto vec = Matrix<int, 2, 1>({2,1});
    auto vec2 = Matrix<int, 1, 2>({1,3});
    auto mat = Matrix<int,2,2>({2,6,1,3});

    decltype(mat) vec3 = vec*vec2;

    REQUIRE( vec3[0][0] == mat[0][0] );
    REQUIRE( vec3[1][1] == mat[1][1] );
    REQUIRE( (vec2*vec)(0,0) == 5 );
}


TEST_CASE( "Scalar MatrixMultiplication", "[scalarmultiplication]" ) {

    auto vec = Matrix<int, 2, 1>({1,1});

    REQUIRE( (2*vec)(0,0) == 2 );
    REQUIRE( (vec*2)(0,0) == 2 );
    REQUIRE( ((vec*2)/2)(0,0) == 1 );
}

TEST_CASE( "Submatrix", "[submatrix]" ) {

    Matrix<int, 3, 3> mat = { 1,9,2,
                              9,0,9,
                              3,9,4};
    auto sub = SubMatrix(mat, 1, 1);
    REQUIRE( sub[0][0] == 1);
    REQUIRE( sub[0][1] == 2 );
    REQUIRE( sub[1][0] == 3 );
    REQUIRE( sub[1][1] == 4 );

}

TEST_CASE( "Determinant", "[determinant]" ) {

    Matrix<int, 1, 1> mat1 = {4};
    Matrix<int, 2, 2> mat2 = { 5,2,
                              2,1};
    Matrix<int, 3, 3> mat3 = { 6,1,1,
                            4,-2,5,
                            2,8,7 };

    Matrix<int, 5, 5> mat5 = {6,1,1,0,0,
                              4,-2,5,0,0,
                              2,8,7,0,0,
                              0,0,0,5,2,
                              0,0,0,2,1};
    REQUIRE( Determinant(mat1) == 4);
    REQUIRE( Determinant(mat2) == 1);
    REQUIRE( Determinant(mat3) == -306);
    REQUIRE( Determinant(mat5) == -306);

}

TEST_CASE( "Inverse", "[inverse]" ) {


    Matrix<int, 2, 2> mat2 = { 5,2,
                              2,1};

    REQUIRE( (Inverse(mat2)*mat2)(0,0) == 1);
    REQUIRE( (Inverse(mat2)*mat2)(1,0) == 0);
    REQUIRE( (Inverse(mat2)*mat2)(0,1) == 0);
    REQUIRE( (Inverse(mat2)*mat2)(1,1) == 1);

}

TEST_CASE( "Equal Scalars", "[equalscalar]" ) {

    int i = 1;
    int j = i+5*std::numeric_limits<int>::epsilon();
    REQUIRE( AlmostEqual(i,j) );

    float x = 1.2343344444f;
    float y = x+5*std::numeric_limits<float>::epsilon();
    REQUIRE( AlmostEqual(x,y) );

    double x2 = 1.23433444323333344;
    double y2 = x2+10*std::numeric_limits<double>::epsilon();
    REQUIRE( AlmostEqual(x2,y2) );
}


TEST_CASE( "Equal Matrices", "[equalmatrix]" ) {

    auto mat = Matrix<float, 2, 2>({ 5,2, 2,1});
    auto mat2 = Matrix<int, 2, 2>({ 5, 2,2,1});
    REQUIRE( mat == mat2 );

    Matrix<double, 2, 2> mat3 = { 5./3.,2./5., 2,3.4};
    Matrix<double, 2, 2>& mat4 = mat3;
    REQUIRE( mat4 == mat3 );

    auto mat5 = Matrix<float, 2, 2>({ 5,2, 2,1});
    Matrix<float, 2, 2> mat6 = { 5+5*std::numeric_limits<float>::epsilon(), 2,
                                2,1+5*std::numeric_limits<float>::epsilon()};
    REQUIRE( mat5 == mat6 );

    Matrix<float, 4, 4> id = {1,0,0,0,
                              0,1,0,0,
                              0,0,1,0,
                              0,0,0,1 };

    Matrix<int, 4, 4> id_int =   {1,0,0,0,
                                  0,1,0,0,
                                  0,0,1,0,
                                  0,0,0,1 };

    REQUIRE( id == id_int );
}

TEST_CASE( "InverseFloat", "[inversefloat]" ) {


    Matrix<float, 4, 4> mat = { 12.3f,4.f,3,4,
                               2,1.43f,0,0,
                               0,32,2.f,0.4f,
                               0.1f,23,3,1};
    Matrix<float, 4, 4> id = {1,0,0,0,
                              0,1,0,0,
                              0,0,1,0,
                              0,0,0,1 };
    REQUIRE( AlmostEqualMatrix(Inverse(mat)*mat, id, 17));
}

TEST_CASE( "InverseDouble", "[inversedouble]" ) {

    Matrix<double, 5, 5> mat5 = { 12.3,4.f,3,4,1,
                               2,1.43f,0,0,2,
                               0,32,2.f,0.4f,3,
                               0.1f,23,3,1,3,
                               4,0,0,0,1};
    Matrix<float, 5, 5> id5 = {1,0,0,0,0,
                              0,1,0,0,0,
                              0,0,1,0,0,
                              0,0,0,1,0,
                              0,0,0,0,1};
    //REQUIRE( Inverse(mat5)*mat5 == id5);
    REQUIRE( AlmostEqualMatrix(Inverse(mat5)*mat5, id5, 17));
}

TEST_CASE("Column Vector Addition", "[rowaddition]"){

    auto vec1 = Vector<int,2>({1,0});
    auto vec2 = Matrix<int, 2, 1>({0,1});

    REQUIRE( vec1+vec2 == Vector<int, 2>({1,1}) );
}

TEST_CASE("Matrix Vector Multiplication", "[MatrixVector]"){

    auto vec = Vec4({1,0,1,0});
    auto mat = Mat4( {3,0,0,0,
                      0,1,0,0,
                      0,0,2,0,
                      0,0,0,1 });

    REQUIRE( mat*vec == Vec4({3,0,2,0}) );
}

TEST_CASE("Identity", "[identity]"){

    auto vec = Vec4({1,0,1,0});

    REQUIRE( id4*vec == vec );
}

TEST_CASE("Cross product", "[cross]"){

    auto vec = Vec3({1,0,0});
    auto vec2 = Vec3({0,1,0});
    auto vec3 = Vec3({0,0,1});

    REQUIRE( Cross(vec,vec2) == vec3 );
    REQUIRE( Cross(vec2,vec3) == vec );
    REQUIRE( Cross(vec,vec3) == -1*vec2 );
    REQUIRE( Cross(vec,vec3)+vec2 == vec-vec );
}

TEST_CASE("Dot product", "[dot]"){

    auto vec = Vec3({1,0,0});
    auto vec2 = Vec3({0,1,0});
    auto vec3 = Vec3({1,2,3});
    auto vec4 = Vec3({1,1,1});

    REQUIRE( Dot(vec,vec2) == 0 );
    REQUIRE( Dot(vec2,vec2) == 1 );
    REQUIRE( Dot(vec3,vec4) == 6 );
}

TEST_CASE("unary minus", "[unaryminus]"){

    auto vec = Vec3({1,2,3});

    REQUIRE( -vec == (-1)*vec );
}

TEST_CASE("Length", "[length]"){

    auto vec = Vec3({1,1,0});
    Vector<int,2> vec2 = {2,0};
    Vector<int,2> vec3 = {1,1};
    REQUIRE( Length(vec) == std::sqrt(2.f) );
    REQUIRE( Length(vec2) == 2 );
    REQUIRE( Length(vec3) == sqrt(2) );
}

TEST_CASE("Normalize", "[normalize]"){

    Vector<int,2> vec2 = {2,0};
    Vector<int,2> vec3 = {1,1};

    REQUIRE( Normalize(vec2) == Vector<float,2>({1,0}) );
    REQUIRE( Normalize(vec3) == Vector<double,2>({1./sqrt(2.),1./sqrt(2.)}) );
}
