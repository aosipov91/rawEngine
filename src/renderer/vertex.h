#ifndef VERTEX_H_
#define VERTEX_H_

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
struct texturedVertex
{
float x, y, z;
float nx, ny, nz;
float tx, ty;
};
struct simpleVertex
{
    glm::vec3 p;
    glm::vec3 n;
};

#endif
