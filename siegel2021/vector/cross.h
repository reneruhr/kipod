#pragma once
#include "vector.h"
namespace siegel2021{

template <typename Vec, typename Tor>
class Cross : public MatrixExpression< Cross<Vec,Tor> >
{
private:
    const Vec& v_;
    const Tor& w_;
public:
    using value_type= std::common_type_t<typename Vec::value_type, typename Tor::value_type>;


    Cross(const Vec& v, const Tor& w) : v_(v), w_(w)
    {
        assert(v_.Rows() == w_.Rows());
        assert(v_.Columns() == w_.Columns());
        assert(v_.Columns() == 1);
        assert(v_.Rows() == 3);
    }

    int Rows() const { return v_.Rows(); }
    int Columns() const { return v_.Columns(); }

    decltype(auto) operator()(int i, int j) const {
        if(i==0) return v_[1]*w_[2] - v_[2]*w_[1];
        if(i==1) return v_[2]*w_[0] - v_[0]*w_[2];
        if(i==2) return v_[0]*w_[1] - v_[1]*w_[0];
    }
    decltype(auto) operator()(int i) const {
        if(i==0) return v_[1]*w_[2] - v_[2]*w_[1];
        if(i==1) return v_[2]*w_[0] - v_[0]*w_[2];
        if(i==2) return v_[0]*w_[1] - v_[1]*w_[0];
    }


};

    }
