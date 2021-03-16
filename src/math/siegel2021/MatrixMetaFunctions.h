#pragma once

namespace siegel2021{
template <typename Scalar, int Rows, int Column>
class Matrix;

template <typename MatrixClass>
class TransposedView;

template <typename MatrixClass>
struct MatrixExpression;

template <typename T>
struct is_matrix : std::false_type {};

template <typename MatrixClass>
struct is_matrix<MatrixExpression<MatrixClass> > : is_matrix<MatrixClass> {};

template <typename T>
struct is_matrix<const T> : is_matrix<T> {};

template <typename Scalar, int Rows, int Columns>
struct is_matrix<Matrix<Scalar, Rows, Columns> > : std::true_type {};


template <typename T>
struct is_square_matrix : std::false_type {};

template <typename Scalar, int Rows>
struct is_square_matrix<Matrix<Scalar, Rows, Rows> > : std::true_type {};

template <typename T>
struct is_square_matrix<const T> : is_square_matrix<T> {};

template <typename MatrixClass>
struct is_square_matrix<TransposedView<MatrixClass> > : is_square_matrix<MatrixClass> {};


template <typename MatrixClass>
inline bool  IsSquare(const MatrixClass& A) {
	return is_square_matrix<MatrixClass>::value;
}


template <typename Mat, typename Rix>
struct same_size_matrices : std::false_type {};

template <typename Scalar, typename Scalar2, int Rows, int Columns>
struct same_size_matrices<Matrix<Scalar, Rows, Columns>, 
						  Matrix<Scalar2, Rows, Columns> > : std::true_type {};

template <typename Mat, typename Rix>
inline bool  AreSameSize(const Mat& A, const Rix& B) {
	return same_size_matrices<Mat,Rix>::value;
}

}