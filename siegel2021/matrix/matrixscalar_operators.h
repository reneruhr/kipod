#pragma once
#include "matrix_expression_template.h"

namespace siegel2021{



template <typename Mat, typename Scalar>
class MatrixScalarProduct : public MatrixExpression< MatrixScalarProduct<Mat, Scalar> >
{
private:
    const Mat& A_;
    const Scalar& B_;
    void check_index(int i, int j) const { assert(0 <= i && i < A_.Rows() && 0 <= j && j < A_.Columns()); }
public:
    using value_type = std::common_type_t<typename Mat::value_type, typename get_value_type<Scalar>::type>;

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
    using value_type = std::common_type_t<typename Mat::value_type, typename get_value_type<Scalar>::type>;

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
    using value_type = std::common_type_t<typename Mat::value_type, typename get_value_type<Scalar>::type>;

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
    using value_type = std::common_type_t<typename Mat::value_type, typename get_value_type<Scalar>::type>;

    ScalarMatrixDivision(const Scalar& A, const Mat& B) : A_(A), B_(B) {}

    int Rows() const { return B_.Rows(); }
    int Columns() const { return B_.Columns(); }

    value_type operator()(int i, int j) const {
        check_index(i, j);
        return  A_ / B_(i, j);
    }
};




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
