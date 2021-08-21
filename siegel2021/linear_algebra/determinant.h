#pragma once
#include "submatrix.h"

namespace siegel2021{

template <typename Mat, int Dim>
struct DeterminantFunctor
{
    decltype(auto) operator()(const Mat& A)
    {
        using value_type = typename Mat::value_type;
        value_type det = 0;
        int sign = 1;
        for (int i = 0; i < Columns(A); i++) {
            auto submatrix = SubMatrix(A, 0, i);
            det += sign * A(0, i) * DeterminantFunctor<decltype(submatrix), Dim - 1>()(submatrix);
            sign = -sign;
        }
        return det;
    }
};

template <typename Mat>
struct DeterminantFunctor< Mat, 1>
{
    decltype(auto) operator()(const Mat& A)
    {
        return A(0, 0);
    }
};

template <typename Mat>
struct DeterminantFunctor<Mat, 2>
{
    decltype(auto) operator()(const Mat& A)
    {
        return A(0, 0)*A(1,1)-A(0,1)*A(1,0);
    }
};

template <typename Mat>
struct DeterminantFunctor<Mat, 3>
{
    decltype(auto) operator()(const Mat& A)
    {
        return
            +A(0,0) * (A(1,1) * A(2,2) - A(1,2) * A(2,1))
            - A(0,1) * (A(1,0) * A(2,2) - A(1,2) * A(2,0))
            + A(0,2) * (A(1,0) * A(2,1) - A(1,1) * A(2,0));
    }
};


template <typename Mat>
struct DeterminantFunctor<Mat, 4>
{
    decltype(auto) operator()(const Mat& A)
    {
        return
            A(0, 3) * A(1, 2) * A(2, 1) * A(3, 0) - A(0, 2) * A(1, 3) * A(2, 1) * A(3, 0) -
            A(0, 3) * A(1, 1) * A(2, 2) * A(3, 0) + A(0, 1) * A(1, 3) * A(2, 2) * A(3, 0) +
            A(0, 2) * A(1, 1) * A(2, 3) * A(3, 0) - A(0, 1) * A(1, 2) * A(2, 3) * A(3, 0) -
            A(0, 3) * A(1, 2) * A(2, 0) * A(3, 1) + A(0, 2) * A(1, 3) * A(2, 0) * A(3, 1) +
            A(0, 3) * A(1, 0) * A(2, 2) * A(3, 1) - A(0, 0) * A(1, 3) * A(2, 2) * A(3, 1) -
            A(0, 2) * A(1, 0) * A(2, 3) * A(3, 1) + A(0, 0) * A(1, 2) * A(2, 3) * A(3, 1) +
            A(0, 3) * A(1, 1) * A(2, 0) * A(3, 2) - A(0, 1) * A(1, 3) * A(2, 0) * A(3, 2) -
            A(0, 3) * A(1, 0) * A(2, 1) * A(3, 2) + A(0, 0) * A(1, 3) * A(2, 1) * A(3, 2) +
            A(0, 1) * A(1, 0) * A(2, 3) * A(3, 2) - A(0, 0) * A(1, 1) * A(2, 3) * A(3, 2) -
            A(0, 2) * A(1, 1) * A(2, 0) * A(3, 3) + A(0, 1) * A(1, 2) * A(2, 0) * A(3, 3) +
            A(0, 2) * A(1, 0) * A(2, 1) * A(3, 3) - A(0, 0) * A(1, 2) * A(2, 1) * A(3, 3) -
            A(0, 1) * A(1, 0) * A(2, 2) * A(3, 3) + A(0, 0) * A(1, 1) * A(2, 2) * A(3, 3);
    }
};

template <typename Mat, typename std::enable_if_t < std::is_base_of_v<MatrixExpression<Mat>, Mat>, void* > = nullptr >
inline
decltype(auto) Determinant(const Mat& A)
    {
        assert(Rows(A) == Columns(A));
        return DeterminantFunctor<Mat, Mat::rows_>()(A);
    }

}
