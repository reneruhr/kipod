#pragma once
#include <cassert>
#include <iostream>

namespace siegel2021{
template <typename Scalar, int Rows, int Column>
class Matrix;

template <typename MatrixClass, typename Scalar>
class RowProxy
{
public:
	friend int Rows(const RowProxy& x) { return MatrixClass::Rows(); }
	friend int Columns(const RowProxy& x) { return MatrixClass::Columns(); }


	RowProxy(MatrixClass& A, int r) : mat_{ A }, r_{ r } {}

	Scalar& operator[](int c) { return mat_(r_, c); }
	const Scalar& operator[](int c) const { return mat_(r_, c); }

	const Scalar& operator=(const Scalar& scalar) {
		assert(Columns(mat_) == 1);
		mat_(r_, 0) = scalar;
		return scalar;
	}


	RowProxy& operator=(const RowProxy& vector) {
		assert(Columns(vector) == Columns(mat_));
		for (int i = 0; i < Columns(mat_); ++i)
			mat_(r_, i) = vector[i];
		return *this;
	}

	template <typename MatrixClass2>
	RowProxy& operator=(const RowProxy<MatrixClass2, Scalar>& vector) {
		assert(Columns(vector) == Columns(mat_));
		for (int i = 0; i < Columns(mat_); ++i)
			mat_(r_, i) = vector[i];
		return *this;
	}
private:
	MatrixClass& mat_;
	int     r_;
};



template <typename MatrixClass, typename Scalar>
inline
std::ostream& operator<<(std::ostream& os, const RowProxy<MatrixClass, Scalar>& vector)
{
	os << '[';
	for (int c = 0; c < MatrixClass::columns_; ++c) {
		os << vector[c] << (c + 1 < MatrixClass::columns_ ? "  " : "");
	}
	os << "]\n";
	return os;
}

}