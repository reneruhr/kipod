#ifndef OBJ_PARSING_H
#define OBJ_PARSING_H

#include <iostream>
#include <vector>
#include "../include/utils/vec.h"

struct FaceIdcs
{
    int v[4];
    int vn[4];
    int vt[4];

    FaceIdcs()
    {
        for (int i=0; i<4; i++)
            v[i] = vn[i] = vt[i] = 0;
    }

    FaceIdcs(std::istream & aStream)
    {
        for (int i=0; i<4; i++)
            v[i] = vn[i] = vt[i] = 0;

        char c;
        for(int i = 0; i < 3; i++)
        {

            aStream >> std::ws >> v[i] >> std::ws;
            if (aStream.peek() != '/')
                continue;
            aStream >> c >> std::ws;
            if (aStream.peek() == '/')
            {
                aStream >> c >> std::ws >> vn[i];
                continue;
            }
            else
                aStream >> vt[i];
            if (aStream.peek() != '/')
                continue;
            aStream >> c >> vn[i];
        }
    }
};

vec3 vec3fFromStream(std::istream & aStream)
{
    float x, y, z;
    aStream >> x >> std::ws >> y >> std::ws >> z;
    return vec3(x, y, z);
}

std::vector<int> vec3iFromStream(std::istream & aStream)
{
    std::vector<int> vec;
    aStream >> vec[0] >> std::ws >> vec[1] >> std::ws >> vec[2];
    return vec;
}

vec2 vec2fFromStream(std::istream & aStream)
{
    float x, y;
    aStream >> x >> std::ws >> y;
    return vec2(x, y);
}






#endif // OBJ_PARSING_H
