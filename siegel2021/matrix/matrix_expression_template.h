#pragma once
#include <cassert>
#include <iostream>
#include <type_traits>
#include "matrix_meta_functions.h"


namespace siegel2021{

template <class MatrixClass> struct MatrixExpression {
    inline auto operator()(size_t i) const {
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




}
