#pragma once
#include "matrix_expression_template.h"
#include "../utils/scalar_comparison.h"
namespace siegel2021{


template <typename Mat, typename Rix>
bool inline AlmostEqualMatrix(const MatrixExpression<Mat>& A, const MatrixExpression<Rix>& B, int ulp=10)
{
    if( not A.Rows() == B.Rows()  &&  A.Columns() == B.Rows()  ) {
        return false;
    }
    using value_type = std::common_type_t<typename Mat::value_type, typename Rix::value_type>;
    for(int i=0; i<A.Rows(); ++i){
        for(int j=0; j<A.Columns(); ++j){
            if(not AlmostEqual(value_type(A(i,j)), value_type(B(i,j)), ulp ) ){
                return false;
            }
        }
    }
    return true;
}

template <typename Mat, typename Rix>
bool inline operator==(const MatrixExpression<Mat>& A, const MatrixExpression<Rix>& B)
{
    return AlmostEqualMatrix(A,B, 10);
}


}
