#pragma once
#include <type_traits>
#include "../matrix/matrix_meta_functions.h"
#include "../matrix/matrix_expression_template.h"

namespace siegel2021{

template <typename Mat, typename std::enable_if_t < std::is_base_of_v<MatrixExpression<Mat>, Mat>, void* >  = nullptr >
inline
decltype(auto) Transpose(const Mat& A)
{
    auto transpose = TransposedView(A);
    return transpose;
}


template <typename MatrixClass>
class TransposedView : public MatrixExpression< TransposedView<MatrixClass> >
{
    static_assert(is_matrix<MatrixClass>::value, "Template argument is not a Matrix");
public:
    using value_type = typename MatrixClass::value_type;
    //friend int Rows(const TransposedView& x) { return MatrixClass::columns_; }
    //friend int Columns(const TransposedView& x) { return MatrixClass::rows_; }
    int Rows() const { return MatrixClass::Columns(); }
    int Columns() const { return MatrixClass::Rows(); }

private:
    using vref_type = std::conditional_t<std::is_const<MatrixClass>::value,	const value_type&,	value_type&>;
public:
    explicit TransposedView(MatrixClass& A) : mat_(A) {}

    vref_type operator()(int r, int c) { return mat_(c, r); }
    const value_type& operator()(int r, int c) const { return mat_(c, r); }
    vref_type operator()(int i) { return mat_(i% MatrixClass::Rows(), i / MatrixClass::Rows()); }
    const value_type& operator()(int i) const { return mat_(i % MatrixClass::rows_, i / MatrixClass::rows_); }

private:
    MatrixClass& mat_;
};



template <typename MatrixClass>
TransposedView<MatrixClass> inline ViewTranspose(MatrixClass& A)
{
    return TransposedView<MatrixClass>(A);
}

}
