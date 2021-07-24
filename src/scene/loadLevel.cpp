#include "loadLevel.h"
#include "src/util/stream.h"
#include "src/renderer/data/mesh.h"
#include "src/renderer/model.h"

#include "src/game/entity.h"

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
        entity->obj.f_vertices = // Each vertex has three floats (x, y, z)
            (struct simpleVertex*)calloc(entity->obj.nVertices, sizeof(struct simpleVertex) );
        stream_read(&stream, entity->obj.f_vertices, entity->obj.nVertices * sizeof(struct simpleVertex));

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
