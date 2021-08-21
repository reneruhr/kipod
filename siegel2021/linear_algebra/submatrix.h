#pragma once
#include "../matrix/matrix_expression_template.h"
namespace siegel2021{

template <typename Mat, typename std::enable_if_t < std::is_base_of_v<MatrixExpression<Mat>, Mat>, void* > = nullptr >
inline
decltype(auto) SubMatrix(const Mat& A, int p, int q)
    {
        assert(Rows(A) > 1 && Columns(A) > 1);
        using value_type = typename Mat::value_type;
        Matrix< value_type, Mat::rows_-1, Mat::columns_-1> submatrix;

        int row_cofac = 0, col_cofac = 0;
        for (int row = 0; row < Rows(A); row++){
            for (int col = 0; col < Columns(A); col++){
                if (row != p && col != q){
                    submatrix(row_cofac, col_cofac++) = A(row,col);
                    if (col_cofac == Columns(A) - 1){
                        col_cofac = 0; row_cofac++;
                    }
                }
            }
        }
        return submatrix;
    }

}
