// http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2013/n3742.pdf
#pragma once
#include <random>

namespace UniformDistribution{

inline auto& global_uniform_random_number_generator()
{
    static std::default_random_engine u{};
    return u;
}

inline void Randomize()
{
    static std::random_device rd{};
    global_uniform_random_number_generator().seed(rd());
}

inline int Pick( int from, int to_including)
{
    static std::uniform_int_distribution<> distribution{};
    using parmesan = decltype(distribution)::param_type;
    return distribution(global_uniform_random_number_generator(), parmesan{from,to_including});
}

inline float Pick(float from, float to_excluding)
{
    static std::uniform_real_distribution<> distribution{};
    using parmesan = decltype(distribution)::param_type;
    return distribution(global_uniform_random_number_generator(), parmesan{from, to_excluding});
}

}

#include "vec.h"

namespace RandomColor{

inline vec4 Pick()
{
    UniformDistribution::Randomize();
    using UniformDistribution::Pick;
    return {Pick(0.0f,1.0f), Pick(0.0f,1.0f), Pick(0.0f,1.0f), 1.0f};
}

}

