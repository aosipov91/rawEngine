#ifndef RANDOM_H_
#define RANDOM_H_

#include "vec3.h"
namespace math {



inline float GetRandomFloat(float a, float b)
{
    if (a >= b)
        return a;
    float f = (rand()%10001) * 0.0001f;

    return (f*(b-a))+a;
}

inline void GetRandomVec(vec3& out)
{
    out.x = GetRandomFloat(-1.0f, 1.0f);
    out.y = GetRandomFloat(-1.0f, 1.0f);
    out.z = GetRandomFloat(-1.0f, 1.0f);

    out = normalized(out);
}

}
#endif
