#pragma once
#include <type_traits>

namespace siegel2021{
template <typename Scalar, int Rows, int Column>
class Matrix;

 template <typename T, typename = void>
 struct get_value_type {
     using type = T;
 };

 template <typename T>
 struct get_value_type<T, std::void_t<typename T::value_type>> {
     using type = typename T::value_type;
 };


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
inline bool  IsSquare(const MatrixClass&) {
	return is_square_matrix<MatrixClass>::value;
}


template <typename Mat, typename Rix>
struct same_size_matrices : std::false_type {};

template <typename Scalar, typename Scalar2, int Rows, int Columns>
struct same_size_matrices<Matrix<Scalar, Rows, Columns>, 
						  Matrix<Scalar2, Rows, Columns> > : std::true_type {};

template <typename Mat, typename Rix>
inline bool  AreSameSize(const Mat&, const Rix&) {
	return same_size_matrices<Mat,Rix>::value;
}

}
