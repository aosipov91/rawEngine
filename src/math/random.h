#ifndef RANDOM_H_
#define RANDOM_H_

#include <glm/vec3.hpp>
namespace math {



inline float GetRandomFloat(float a, float b)
{
    if (a >= b)
        return a;
    float f = (rand()%10001) * 0.0001f;

    return (f*(b-a))+a;
}

inline void GetRandomVec(glm::vec3& out)
{
    out.x = GetRandomFloat(-1.0f, 1.0f);
    out.y = GetRandomFloat(-1.0f, 1.0f);
    out.z = GetRandomFloat(-1.0f, 1.0f);

    out = glm::normalize(out);
}

}
#endif
