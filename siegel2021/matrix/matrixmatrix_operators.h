#pragma once
#include "matrix_expression_template.h"

namespace siegel2021{

template <typename Mat, typename Rix>
class MatrixSum : public MatrixExpression< MatrixSum<Mat,Rix> >
{
private:
    const Mat& A_;
    const Rix& B_;
    void check_index(int i, int j) const { assert(0 <= i && i < A_.Rows() && 0 <= j && j < A_.Columns()); }
    void check_index(int i) const { assert(0 <= i && i < A_.Rows()* A_.Columns()); }
public:
    using value_type= std::common_type_t<typename Mat::value_type, typename Rix::value_type>;


    MatrixSum(const Mat& A, const Rix& B) : A_(A), B_(B)
    {
        assert(A_.Rows() == B_.Rows() && A_.Columns() == B_.Columns());
    }


    int Rows() const { return A_.Rows(); }
    int Columns() const { return A_.Columns(); }

    decltype(auto) operator()(int i, int j) const { check_index(i,j); return A_(i,j) + B_(i,j); }
    decltype(auto) operator()(int i) const { check_index(i); return A_(i) + B_(i); }


};

template <typename Mat, typename Rix>
class MatrixSubstraction : public MatrixExpression< MatrixSubstraction<Mat, Rix> >
{
private:
    const Mat& A_;
    const Rix& B_;
    void check_index(int i, int j) const { assert(0 <= i && i < A_.Rows() && 0 <= j && j < A_.Columns()); }
    void check_index(int i) const { assert(0 <= i && i < A_.Rows()* A_.Columns()); }
public:
    using value_type = std::common_type_t<typename Mat::value_type, typename Rix::value_type>;


    MatrixSubstraction(const Mat& A, const Rix& B) : A_(A), B_(B)
    {
        assert(A_.Rows() == B_.Rows() && A_.Columns() == B_.Columns());
    }


    int Rows() const { return A_.Rows(); }
    int Columns() const { return A_.Columns(); }

    decltype(auto) operator()(int i, int j) const { check_index(i, j); return A_(i, j) - B_(i, j); }
    decltype(auto) operator()(int i) const { check_index(i); return A_(i) - B_(i); }


};


template <typename Mat, typename Rix>
class MatrixProduct : public MatrixExpression< MatrixProduct<Mat, Rix> >
{
private:
    const Mat& A_;
    const Rix& B_;
    void check_index(int i, int j) const { assert(0 <= i && i < A_.Rows() && 0 <= j && j < B_.Columns()); }
public:
    using value_type = std::common_type_t<typename Mat::value_type, typename Rix::value_type>;

    MatrixProduct(const Mat& A, const Rix& B) : A_(A), B_(B)
    {
        assert(A_.Columns() == B_.Rows());
    }


    int Rows() const { return A_.Rows(); }
    int Columns() const { return B_.Columns(); }

    value_type operator()(int i, int j) const {
        check_index(i, j);
        value_type out{ 0 };
        for (int k = 0; k < A_.Columns(); ++k) out += A_(i, k) * B_(k, j);
        return out;
    }

};



template <typename Mat, typename Rix>
MatrixSum<Mat, Rix> inline const operator+(const MatrixExpression<Mat>& A, const MatrixExpression<Rix>& B)
{
    return MatrixSum<Mat, Rix>(A, B);
}

template <typename Mat, typename Rix>
MatrixSubstraction<Mat, Rix> inline const operator-(const MatrixExpression<Mat>& A, const MatrixExpression<Rix>& B)
{
    return MatrixSubstraction<Mat, Rix>(A, B);
}

template <typename Mat, typename Rix>
MatrixProduct<Mat, Rix> inline const operator*(const MatrixExpression<Mat>& A, const MatrixExpression<Rix>& B)
{
    return MatrixProduct<Mat, Rix>(A, B);
}
}
