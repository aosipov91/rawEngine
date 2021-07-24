#ifndef MESH_H_
#define MESH_H_

#include "src/renderer/opengl/openGLExtensions.h"
#include "src/renderer/vertex.h"

typedef struct Mesh
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    int nIndices;
} Mesh;

Mesh*
create_mesh_simple_vertex(const struct simpleVertex* vertices, int nVertices,
	const unsigned int* indices, int nIndices);
Mesh*
create_textured_mesh(const struct texturedVertex* vertices, int nVertices, const unsigned int* indices, int nIndices);
Mesh*
create_mesh(const float* vertices, int nVertices,
            const unsigned int* indices,
            int nIndices);
void draw_mesh(const Mesh* mesh);
void destroy_mesh(Mesh* mesh);

#endif
