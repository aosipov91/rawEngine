#ifndef RANDOM_H_
#define RANDOM_H_

#include "vec3.h"
namespace math {

const vec3 WHITE = vec3(255, 255, 255);
const vec3 BLACK = vec3(0, 0, 0);
const vec3 RED = vec3(255, 0, 0);
const vec3 GREEN = vec3(0, 255, 0);
const vec3 BLUE = vec3(0, 0, 255);
const vec3 YELLOW = vec3(255, 255, 0);
const vec3 CYAN = vec3(0, 255, 255);
const vec3 MAGENTA = vec3(255, 0, 255);

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
