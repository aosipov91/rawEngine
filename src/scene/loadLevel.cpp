#include "loadLevel.h"
#include "src/util/stream.h"
#include "src/renderer/data/mesh.h"
#include "src/renderer/model.h"

#include "src/game/entity.h"

bool LoadLevelTextured(const char* filename)
{
    struct Stream stream{};
    stream_open(&stream, filename);

    while (stream.pos < stream.size)
    {
        entity_t* entity = ( entities[entityCount++] = (entity_t*)calloc(1, sizeof(entity_t)) );
        entity_init(entity);

        stream_read(&stream, &entity->obj.type, sizeof(entity->obj.type));
        stream_read(&stream, &entity->obj.matrix, sizeof(entity->obj.matrix));

        if (entity->obj.type != MESH)
        {
            continue;
        }

        //stream_read(&stream, &entity->obj.textured, sizeof(entity->obj.textured));

            char* tex1 = stream_read_str(&stream);
            //stream_read(&stream, &entity->obj.textured, sizeof(entity->obj.textured));
            char* tex2 = stream_read_str(&stream);
            //stream_read(&stream, &entity->obj.textured, sizeof(entity->obj.textured));
            char* tex3 = stream_read_str(&stream);

            //if  (tex1) entity->obj.diffuseMap = new Texture(tex1);
            //if  (tex2) entity->obj.normalMap = new Texture(tex2);
            //if  (tex3) entity->obj.specularMap = new Texture(tex3);


            //free(tex1);
            //free(tex2);
            //free(tex3);
            delete[] tex1;
            delete[] tex2;
            delete[] tex3;



        stream_read(&stream, &entity->obj.nIndices, sizeof(entity->obj.nIndices));
        entity->obj.indices = (unsigned*)calloc(entity->obj.nIndices, sizeof(unsigned));// new unsigned[entity->obj.nIndices];
        stream_read(&stream, entity->obj.indices, entity->obj.nIndices * sizeof(unsigned int));

        // it's actually num of element in array of vertices

        stream_read(&stream, &entity->obj.nVertices, sizeof(entity->obj.nVertices)); // it's actuall num of elements in
        entity->obj.textured_vertices = (float*)calloc(entity->obj.nVertices, sizeof(float)); // Each vertex has three floats (x, y, z)
        stream_read(&stream, entity->obj.textured_vertices, entity->obj.nVertices * sizeof(float));

    }
    return true;
}

bool LoadCollidableGeometry(const char* filename)
{
    struct Stream stream{};
    stream_open(&stream, filename);
    while (stream.pos < stream.size)
    {
        entity_t* entity = ( entities[entityCount++] = (entity_t*)calloc(1, sizeof(entity_t)) );
        entity_init(entity);

        stream_read(&stream, &entity->obj.type, sizeof(entity->obj.type));
        stream_read(&stream, &entity->obj.matrix, sizeof(entity->obj.matrix));

        if (entity->obj.type != MESH) continue;

        stream_read(&stream, &entity->obj.nIndices, sizeof(entity->obj.nIndices));
        entity->obj.indices = 
            (unsigned*)calloc(entity->obj.nIndices, sizeof(unsigned));
        stream_read(&stream, entity->obj.indices, entity->obj.nIndices * sizeof(unsigned int));

        stream_read(&stream, &entity->obj.nVertices, sizeof(entity->obj.nVertices));
        entity->obj.f_vertices = // Each vertex has three floats (px, py, pz, nx, ny, nz, tx, ty)
            (struct simpleVertex*)calloc(entity->obj.nVertices, sizeof(struct simpleVertex) );
        stream_read(&stream, entity->obj.f_vertices, entity->obj.nVertices * sizeof(simpleVertex));

    }
    return true;
}

void ClearLevel()
{
    for (int i = 0; i < entityCount; i++)
    {
        entity_remove(entities[i]);
        free(entities[i]);
    }
}
