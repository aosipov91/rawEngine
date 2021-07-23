#ifndef CONSTANT_H_
#define CONSTANT_H_

#include "vec3.h"

namespace math {
float constexpr PI = 3.14159265358979323846f;
float constexpr DEG2RAD = PI / 180.0f;
float constexpr RAD2DEG = 180.0f / PI;

const vec3 WHITE = vec3(255, 255, 255);
const vec3 BLACK = vec3(0, 0, 0);
const vec3 RED = vec3(255, 0, 0);
const vec3 GREEN = vec3(0, 255, 0);
const vec3 BLUE = vec3(0, 0, 255);
const vec3 YELLOW = vec3(255, 255, 0);
const vec3 CYAN = vec3(0, 255, 255);
const vec3 MAGENTA = vec3(255, 0, 255);
}

#endif
