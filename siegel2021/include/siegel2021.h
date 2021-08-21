#pragma once
#include "../matrix/matrix.h"
#include "../linear_algebra/transpose.h"
#include "../linear_algebra/inverse.h"

#include "../vector/vector.h"
#include "../vector/cross.h"


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
