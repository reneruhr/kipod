#pragma once
#include <cassert>
#include <iostream>
#include <type_traits>
#include "MatrixMetaFunctions.h"

namespace siegel2021{
template <class MatrixClass> struct MatrixExpression {
    inline const auto operator()(size_t i) const {
        return static_cast<MatrixClass const&>(*this)(i);
    }
    decltype(auto) operator()(int i, int j) const { 
        return static_cast<MatrixClass const&>(*this)(i, j);
    }

    operator MatrixClass& () {
        return static_cast<MatrixClass&>(*this);
    }
    operator const MatrixClass& () const {
        return static_cast<const MatrixClass&>(*this);
    }
    

    int Rows()
    {
        return static_cast<MatrixClass&>(*this).Rows();
    }
    int Columns()
    {
        return static_cast<MatrixClass&>(*this).Columns();
    }
    int Rows() const
    {
        return static_cast<const MatrixClass&>(*this).Rows();
    }
    int Columns() const
    {
        return static_cast<const MatrixClass&>(*this).Columns();
    }

    friend int Rows(const MatrixExpression& A)
    {
        return A.Rows();
    }
    friend int Columns(const MatrixExpression& A)
    {
        return A.Columns();
    }
};

template <typename MatrixClass>
std::ostream& operator<<(std::ostream& os, const MatrixExpression<MatrixClass>& A)
{
    for (int r = 0; r < Rows(A); ++r) {
        os << '[';
        for (int c = 0; c < Columns(A); ++c)
            os << A(r, c) << (c + 1 < Columns(A) ? "  " : "");
        os << "]\n";
    }
    return os;
}



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




template <typename Mat, typename Scalar>
class MatrixScalarProduct : public MatrixExpression< MatrixScalarProduct<Mat, Scalar> >
{
private:
    const Mat& A_;
    const Scalar& B_;
    void check_index(int i, int j) const { assert(0 <= i && i < A_.Rows() && 0 <= j && j < A_.Columns()); }
public:
    using value_type = std::common_type_t<typename Mat::value_type, typename Scalar>;

    MatrixScalarProduct(const Mat& A, const Scalar& B) : A_(A), B_(B){}

    int Rows() const { return A_.Rows(); }
    int Columns() const { return A_.Columns(); }

    value_type operator()(int i, int j) const {
        check_index(i, j);
        return  A_(i, j) * B_;
    }

};

template <typename Scalar, typename Mat>
class ScalarMatrixProduct : public MatrixExpression< ScalarMatrixProduct<Scalar, Mat> >
{
private:
    const Scalar& A_;
    const Mat& B_;
    void check_index(int i, int j) const { assert(0 <= i && i < B_.Rows() && 0 <= j && j < B_.Columns()); }
public:
    using value_type = std::common_type_t<typename Mat::value_type, typename Scalar>;

    ScalarMatrixProduct(const Scalar& A, const Mat& B) : A_(A), B_(B) {}

    int Rows() const { return B_.Rows(); }
    int Columns() const { return B_.Columns(); }

    value_type operator()(int i, int j) const {
        check_index(i, j);
        return  A_ * B_(i, j);
    }
};


template <typename Mat, typename Scalar>
class MatrixScalarDivision : public MatrixExpression< MatrixScalarDivision<Mat, Scalar> >
{
private:
    const Mat& A_;
    const Scalar& B_;
    void check_index(int i, int j) const { assert(0 <= i && i < A_.Rows() && 0 <= j && j < A_.Columns()); }
public:
    using value_type = std::common_type_t<typename Mat::value_type, typename Scalar>;

    MatrixScalarDivision(const Mat& A, const Scalar& B) : A_(A), B_(B) {}

    int Rows() const { return A_.Rows(); }
    int Columns() const { return A_.Columns(); }

    value_type operator()(int i, int j) const {
        check_index(i, j);
        return  A_(i, j) / B_;
    }

};

template <typename Scalar, typename Mat>
class ScalarMatrixDivision : public MatrixExpression< ScalarMatrixDivision<Scalar, Mat> >
{
private:
    const Scalar& A_;
    const Mat& B_;
    void check_index(int i, int j) const { assert(0 <= i && i < B_.Rows() && 0 <= j && j < B_.Columns()); }
public:
    using value_type = std::common_type_t<typename Mat::value_type, typename Scalar>;

    ScalarMatrixDivision(const Scalar& A, const Mat& B) : A_(A), B_(B) {}

    int Rows() const { return B_.Rows(); }
    int Columns() const { return B_.Columns(); }

    value_type operator()(int i, int j) const {
        check_index(i, j);
        return  A_ / B_(i, j);
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

template <typename Mat, typename Scalar>
typename std::enable_if_t<!std::is_base_of_v<MatrixExpression<Scalar>, Scalar>,
    MatrixScalarProduct<Mat, Scalar >>  const operator*(const MatrixExpression<Mat>& A, const Scalar& B)
{
    return MatrixScalarProduct<Mat, Scalar>(A, B);
}

template <typename Scalar, typename Mat>
typename std::enable_if_t<!std::is_base_of_v<MatrixExpression<Scalar>, Scalar>,
    ScalarMatrixProduct<Scalar, Mat >>  const operator*(const Scalar& A, const MatrixExpression<Mat>& B)
{
    return ScalarMatrixProduct<Scalar, Mat>(A, B);
}


template <typename Mat, typename Scalar>
typename std::enable_if_t<!std::is_base_of_v<MatrixExpression<Scalar>, Scalar>,
    MatrixScalarDivision<Mat, Scalar >>  const operator/(const MatrixExpression<Mat>& A, const Scalar& B)
{
    return MatrixScalarDivision<Mat, Scalar>(A, B);
}

template <typename Scalar, typename Mat>
typename std::enable_if_t<!std::is_base_of_v<MatrixExpression<Scalar>, Scalar>,
    ScalarMatrixDivision<Scalar, Mat >>  const operator/(const Scalar& A, const MatrixExpression<Mat>& B)
{
    return ScalarMatrixDivision<Scalar, Mat>(A, B);
}

}