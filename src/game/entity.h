#ifndef ENTITY_H_
#define ENTITY_H_

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include "src/renderer/model.h"

#define MAX_ENTITIES 1024

typedef struct entity_s
{
    struct Model obj;

    //Sphere s;
		glm::vec3 pos;
		glm::vec3 rot;
		glm::vec3 velocity;

		glm::mat4 mWorldTransform;
} entity_t;

extern entity_t* entities[MAX_ENTITIES];
extern int entityCount;


void entity_init(entity_t *entity);
void entity_remove(entity_t *entity);

#endif
