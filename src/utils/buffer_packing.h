#ifndef BUFFER_PACKING_H
#define BUFFER_PACKING_H

#include <vector>
#include "vec.h"
#include <assert.h>
#include "log.h"

inline void pack_vectors(const std::vector<vec3>& a, const std::vector<vec3>& b, std::vector<vec3>& c,
                  const std::vector<unsigned int>& in_a, const std::vector<unsigned int>& in_b, std::vector<unsigned int>& in_c)
{
    LOG_ENGINE("Pack vectors");
    assert(in_a.size() == in_b.size());
    for(unsigned int i = 0, n = in_a.size(); i<n; i++)
    {
        c.push_back(a[in_a[i]]);
        c.push_back(b[in_b[i]]);
        in_c.push_back(i);
    }
}


#endif // BUFFER_PACKING_H
