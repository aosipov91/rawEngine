#ifndef MODEL_H_
#define MODEL_H_

#include "src/renderer/opengl/openGLExtensions.h"

#include <glm/matrix.hpp>
#include "src/renderer/data/mesh.h"
#include "src/renderer/texture.h"
enum Type {
    MESH,
    LAMP,
    RESPAWN,
};

struct Model
{
    enum Type type;
    glm::mat4 matrix;
    int   nIndices;
    int   nVertices;
    unsigned int   *indices;
    struct simpleVertex* f_vertices;
    struct texturedVertex* t_vertices;
    float *textured_vertices;
    int textured;

    uint32_t diffuseMap;
    uint32_t specularMap;
    uint32_t normalMap;

};


void model_init(struct Model *model);
void model_unload(struct Model *model);

#endif
