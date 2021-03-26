#pragma once
#include "determinant.h"
namespace siegel2021{

    template <typename Mat, int Dim>
    struct InverseFunctor
    {
        decltype(auto) operator()(const Mat& A)
        {
            using value_type = typename Mat::value_type;
            value_type det = Determinant(A);
            assert(det != 0);
            Matrix< value_type, Mat::rows_, Mat::columns_> inverse;

            for (int row = 0; row < Rows(A); row++) {
                for (int col = 0; col < Columns(A); col++) {
                    int sign = (row + col) % 2 == 0 ? 1 : -1;
                    inverse(row, col) = sign * Determinant(SubMatrix(A, col, row)) / det;
                }
            }
            return inverse;
        }
    };

    template <typename Mat>
    struct InverseFunctor<Mat, 2>
    {
        decltype(auto) operator()(const Mat& A)
        {
            using value_type = typename Mat::value_type;
            value_type det = Determinant(A);
            assert(det != 0);
            Matrix< value_type, Mat::rows_, Mat::columns_> inverse = { A(1,1) / det, -A(0,1) / det,
                                                                           -A(1,0) / det, A(0,0) / det };
            return inverse;
        }
    };

    template <typename Mat>
    struct InverseFunctor<Mat, 3>
    {
        decltype(auto) operator()(const Mat& A)
        {
            using value_type = typename Mat::value_type;
            value_type det = Determinant(A);
            assert(det != 0);
            Matrix< value_type, Mat::rows_, Mat::columns_> inverse;
            inverse(0,0) = +(A(1,1) * A(2,2) - A(2,1) * A(1,2));
            inverse(1,0) = -(A(1,0) * A(2,2) - A(2,0) * A(1,2));
            inverse(2,0) = +(A(1,0) * A(2,1) - A(2,0) * A(1,1));
            inverse(0,1) = -(A(0,1) * A(2,2) - A(2,1) * A(0,2));
            inverse(1,1) = +(A(0,0) * A(2,2) - A(2,0) * A(0,2));
            inverse(2,1) = -(A(0,0) * A(2,1) - A(2,0) * A(0,1));
            inverse(0,2) = +(A(0,1) * A(1,2) - A(1,1) * A(0,2));
            inverse(1,2) = -(A(0,0) * A(1,2) - A(1,0) * A(0,2));
            inverse(2,2) = +(A(0,0) * A(1,1) - A(1,0) * A(0,1));

            inverse /= det;
            return inverse;
        }
    };

    template <typename Mat>
    struct InverseFunctor<Mat, 4>
    {
        decltype(auto) operator()(const Mat& A)
        {
            using value_type = typename Mat::value_type;
            Matrix< value_type, Mat::rows_, Mat::columns_> inverse;

            auto subfactor00 = A(2,2) * A(3,3) - A(3,2) * A(2,3);
            auto subfactor01 = A(2,1) * A(3,3) - A(3,1) * A(2,3);
            auto subfactor02 = A(2,1) * A(3,2) - A(3,1) * A(2,2);
            auto subfactor03 = A(2,0) * A(3,3) - A(3,0) * A(2,3);
            auto subfactor04 = A(2,0) * A(3,2) - A(3,0) * A(2,2);
            auto subfactor05 = A(2,0) * A(3,1) - A(3,0) * A(2,1);
            auto subfactor06 = A(1,2) * A(3,3) - A(3,2) * A(1,3);
            auto subfactor07 = A(1,1) * A(3,3) - A(3,1) * A(1,3);
            auto subfactor08 = A(1,1) * A(3,2) - A(3,1) * A(1,2);
            auto subfactor09 = A(1,0) * A(3,3) - A(3,0) * A(1,3);
            auto subfactor10 = A(1,0) * A(3,2) - A(3,0) * A(1,2);
            auto subfactor11 = A(1,0) * A(3,1) - A(3,0) * A(1,1);
            auto subfactor12 = A(1,2) * A(2,3) - A(2,2) * A(1,3);
            auto subfactor13 = A(1,1) * A(2,3) - A(2,1) * A(1,3);
            auto subfactor14 = A(1,1) * A(2,2) - A(2,1) * A(1,2);
            auto subfactor15 = A(1,0) * A(2,3) - A(2,0) * A(1,3);
            auto subfactor16 = A(1,0) * A(2,2) - A(2,0) * A(1,2);
            auto subfactor17 = A(1,0) * A(2,1) - A(2,0) * A(1,1);


            inverse(0,0) = +(A(1,1) * subfactor00 - A(1,2) * subfactor01 + A(1,3) * subfactor02);
            inverse(1,0) = -(A(1,0) * subfactor00 - A(1,2) * subfactor03 + A(1,3) * subfactor04);
            inverse(2,0) = +(A(1,0) * subfactor01 - A(1,1) * subfactor03 + A(1,3) * subfactor05);
            inverse(3,0) = -(A(1,0) * subfactor02 - A(1,1) * subfactor04 + A(1,2) * subfactor05);

            inverse(0,1) = -(A(0,1) * subfactor00 - A(0,2) * subfactor01 + A(0,3) * subfactor02);
            inverse(1,1) = +(A(0,0) * subfactor00 - A(0,2) * subfactor03 + A(0,3) * subfactor04);
            inverse(2,1) = -(A(0,0) * subfactor01 - A(0,1) * subfactor03 + A(0,3) * subfactor05);
            inverse(3,1) = +(A(0,0) * subfactor02 - A(0,1) * subfactor04 + A(0,2) * subfactor05);

            inverse(0,2) = +(A(0,1) * subfactor06 - A(0,2) * subfactor07 + A(0,3) * subfactor08);
            inverse(1,2) = -(A(0,0) * subfactor06 - A(0,2) * subfactor09 + A(0,3) * subfactor10);
            inverse(2,2) = +(A(0,0) * subfactor07 - A(0,1) * subfactor09 + A(0,3) * subfactor11);
            inverse(3,2) = -(A(0,0) * subfactor08 - A(0,1) * subfactor10 + A(0,2) * subfactor11);

            inverse(0,3) = -(A(0,1) * subfactor12 - A(0,2) * subfactor13 + A(0,3) * subfactor14);
            inverse(1,3) = +(A(0,0) * subfactor12 - A(0,2) * subfactor15 + A(0,3) * subfactor16);
            inverse(2,3) = -(A(0,0) * subfactor13 - A(0,1) * subfactor15 + A(0,3) * subfactor17);
            inverse(3,3) = +(A(0,0) * subfactor14 - A(0,1) * subfactor16 + A(0,2) * subfactor17);

            auto det =
                  A(0,0) * inverse(0,0)
                + A(0,1) * inverse(1,0)
                + A(0,2) * inverse(2,0)
                + A(0,3) * inverse(3,0);

            assert(det != 0);
            inverse /= det;

            return inverse;
        }
    };



    template <typename Mat, typename std::enable_if_t < std::is_base_of_v<MatrixExpression<Mat>, Mat>, void* > = nullptr >
    inline
    decltype(auto) Inverse(const Mat& A)
    {
        return InverseFunctor<Mat, Mat::rows_>()(A);
    }


    }
