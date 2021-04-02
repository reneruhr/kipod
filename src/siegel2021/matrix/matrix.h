#pragma once
#include <cassert>
#include <iostream>
#include <initializer_list>
#include <array>

#include "matrix_proxy.h"
#include "matrix_expression_template.h"
#include "matrixmatrix_operators.h"
#include "matrixscalar_operators.h"
#include "matrix_comparison.h"

namespace siegel2021{


template <typename Scalar, int RowSize, int ColumnSize= RowSize>
class Matrix : public MatrixExpression< Matrix<Scalar, RowSize, ColumnSize> >
{
private:
	std::array<Scalar, RowSize* ColumnSize> data_;
public:
	using value_type = Scalar;
	using size_type = size_t;
	using const_RowProxy = RowProxy<const Matrix, const Scalar>;

	static int const rows_ = RowSize;
	static int const columns_ = ColumnSize;
	static int Rows()  { return RowSize; }
	static int Columns()  { return ColumnSize; }


	Matrix() : data_{ Scalar(0) } {};
	Matrix(const Matrix& other) = default;
	Matrix(Matrix&& other) = default;
	Matrix& operator=(const Matrix& other) = default;
	Matrix& operator=(Matrix&& other) = default;
	~Matrix() = default;
	
	Matrix(std::initializer_list<Scalar> data) : data_{ Scalar(0) } {
		using namespace std;
		assert(size(data) <= size(data_));
		copy(begin(data), end(data), begin(data_));
	}



	template <typename Source>
	Matrix& operator=(const MatrixExpression<Source>& source)
	{
		CheckSize(source);
		for (int i = 0; i < Rows(); ++i)
			for (int j = 0; j < Columns(); ++j)
				data_[MatrixIndex(i,j)] = source(i,j);
		return *this;
	}
	template <typename Source>
	Matrix(const MatrixExpression<Source>& source)
	{
		CheckSize(source);
		for (int i = 0; i < Rows(); ++i)
			for (int j = 0; j < Columns(); ++j)
				data_[MatrixIndex(i, j)] = source(i, j);
	}

    template <typename T>
    Matrix& operator/=(const T& scalar)
	{
		for (int i = 0; i < Rows(); ++i)
			for (int j = 0; j < Columns(); ++j)
				data_[MatrixIndex(i, j)] /=scalar;
		return *this;
	}

    template <typename T>
    Matrix& operator*=(const T& scalar)
	{
		for (int i = 0; i < Rows(); ++i)
			for (int j = 0; j < Columns(); ++j)
				data_[MatrixIndex(i, j)] *= scalar;
		return *this;
	}

	
    Matrix operator-()
	{
        Matrix minus;
		for (int i = 0; i < Rows(); ++i)
			for (int j = 0; j < Columns(); ++j)
				minus(i,j) = -data_[MatrixIndex(i, j)];
		return minus;
	}



	Scalar& operator()(int i, int j) {
		CheckIndex(i, j);
		return data_[MatrixIndex(i, j)];
	}
	const Scalar& operator()(int i, int j) const{
		CheckIndex(i, j);
		return data_[MatrixIndex(i, j)];
	}
	Scalar& operator()(int i) {
		CheckIndex(i);
		return data_[i];
	}
	const Scalar& operator()(int i) const {
		CheckIndex(i);
		return data_[i];
	}

	RowProxy<Matrix, Scalar> operator[](int r)
	{
		return { *this, r };
	}

	const_RowProxy operator[](int r) const
	{
		return { *this, r };
	}



private:

	void CheckIndex(int i, int j) const
	{
		assert(0 <= i && i < Rows() && 0 <= j && j < Columns());
	}
	void CheckIndex(int i) const
	{
		assert(0 <= i && i < Rows()*Columns());
	}

	inline static int MatrixIndex(int i, int j)
	{
		return i * Columns() + j;
	}

	template<typename Source>
	void CheckSize(const MatrixExpression<Source>& source) const { 
		assert(this->Rows() == source.Rows() && this->Columns() == source.Columns());
	}
};


}
