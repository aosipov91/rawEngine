#include "mesh.h"



#include <stdlib.h>

Mesh*
create_mesh_simple_vertex(const struct simpleVertex* vertices, int nVertices,
	const unsigned int* indices, int nIndices)
{
    Mesh* mesh = NULL;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(struct simpleVertex), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    float* ptr = 0;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct simpleVertex), (GLvoid*)(ptr += 0)); // position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct simpleVertex), (GLvoid*)(ptr += 3)); // normal
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    glBindVertexArray(0);

    mesh = (struct Mesh*)calloc(1, sizeof(struct Mesh));
    mesh->vao = vao;
    mesh->vbo = vbo;
    mesh->ebo = ebo;
    mesh->nIndices = nIndices;

    return mesh;
}

Mesh*
create_textured_mesh(const struct texturedVertex* vertices, int nVertices, const unsigned int* indices, int nIndices)
{
    Mesh* mesh = NULL;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(texturedVertex), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    float* ptr = 0;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(texturedVertex) , (GLvoid*)(ptr += 0)); // position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(texturedVertex),  (GLvoid*)(ptr += 3)); // normal
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(texturedVertex), (GLvoid*)(ptr += 3)); // texcoords

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);

    mesh = (struct Mesh*)calloc(1, sizeof(struct Mesh));
    mesh->vao = vao;
    mesh->vbo = vbo;
    mesh->ebo = ebo;
    mesh->nIndices = nIndices;

    return mesh;
}


Mesh*
create_mesh(const float* vertices,
            int nVertices,
            const unsigned int* indices,
            int nIndices)
{
    Mesh* mesh = NULL;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(float) * 5, 0, GL_STATIC_DRAW);
    float* pData = static_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    //memcpy(pData, vertices, nVertices * sizeof(float) * 5);
    for (int i = 0; i < nVertices; i++)
    {
        *(pData++) = vertices[i * 5 + 0];
        *(pData++) = vertices[i * 5 + 1];
        *(pData++) = vertices[i * 5 + 2];
        *(pData++) = vertices[i * 5 + 3];
        *(pData++) = vertices[i * 5 + 4];
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    //glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(float) * 5, vertices, GL_STATIC_DRAW);
    float *ptr = 0;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, ptr+=0); // position
    glEnableVertexAttribArray( 0 );

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, ptr+=3); // position
    glEnableVertexAttribArray( 1 );

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    mesh = (struct Mesh*)calloc(1, sizeof(struct Mesh));
    mesh->vao = vao;
    mesh->vbo = vbo;
    mesh->ebo = ebo;
    mesh->nIndices = nIndices;

    return mesh;
}

void draw_mesh(const Mesh* mesh)
{
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->nIndices, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}
void destroy_mesh(Mesh* mesh)
{
    glBindVertexArray(mesh->vao);
    /* отцепляем буфер */
    // TODO: correct way for clear buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &mesh->ebo);
    /* делаем неактивным массив вершин */
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &mesh->vao);
}
