#pragma once
#include <limits>
#include <cmath>
#include <algorithm>
#include <iostream>

template<typename T>
typename std::enable_if_t<!std::numeric_limits<T>::is_integer, bool>
AlmostEqual(T x, T y, int units_least_precision = 10)
{
	T diff = std::abs(x-y);
    //if (diff < 1000 * std::numeric_limits<T>::min())
    if (diff < units_least_precision * std::numeric_limits<T>::epsilon())
    	return true;
    
    float max = std::max(std::abs(x), std::abs(y));

    bool return_val = diff / max <= std::numeric_limits<T>::epsilon() *  units_least_precision;

//    if(!return_val)
//    {
//        std::cout << "x= " << x << " y= " << y << " diff = " << diff << " max = " << max << '\n';
//        std::cout << "diff / max  = " << diff / max << "\n";
//        std::cout << "num_lim * ulp =  " << std::numeric_limits<T>::epsilon() *  units_least_precision << '\n';
//        std::cout << "1000 * std::numeric_limits<T>::min() = " <<  1000 * std::numeric_limits<T>::min() << "\n";

//    }

    return return_val;
}

template<typename T>
typename std::enable_if_t<std::numeric_limits<T>::is_integer, bool>
AlmostEqual(T x, T y)
{
    return x==y;
}

template<typename T>
typename std::enable_if_t<std::numeric_limits<T>::is_integer, bool>
AlmostEqual(T x, T y, int)
{
    return x==y;
}
