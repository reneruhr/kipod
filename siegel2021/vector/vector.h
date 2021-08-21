#pragma once
#include "../matrix/matrix.h"

namespace siegel2021 {
template<typename Scalar, int dim>
using Vector = Matrix<Scalar,dim,1>;


template <typename Vec, typename Tor>
decltype(auto) Dot(Vec& v, Tor& w){
    using value_type= std::common_type_t<typename Vec::value_type, typename Tor::value_type>;

    value_type dot = value_type(0);
    for(int i = 0; i<v.Rows(); ++i) dot+=v[i]*w[i];
    return dot;
}

template <typename Scalar, int dim>
decltype(auto) Length(Vector<Scalar, dim> v){
        auto l = v[0]*v[0];
        for(int i = 1; i< dim; ++i) l+=v[i]*v[i];
        return std::sqrt(l);
}

template <typename Scalar, int dim>
decltype(auto) Normalize(Vector<Scalar, dim> v){
        auto l = Length(v);
        assert(l!=0);
        Vector<decltype(l),dim> w = v/l;
        return w;
}

}
