#pragma once
#include "siegel2021/matrix/matrix.h"
#include "siegel2021/vector/vector.h"
#include "siegel2021/linear_algebra/transpose.h"
#include "siegel2021/linear_algebra/inverse.h"
namespace siegel2021{

    using Mat4 = Matrix<float,4>;
    using Mat3 = Matrix<float,3>;
    using Mat2 = Matrix<float,2>;

    using Vec4 = Vector<float,4>;
    using Vec3 = Vector<float,3>;
    using Vec2 = Vector<float,2>;

    const Matrix<int,4> id4({1,0,0,0,
                              0,1,0,0,
                              0,0,1,0,
                              0,0,0,1 });
}
