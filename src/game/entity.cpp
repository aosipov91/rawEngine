#include "entity.h"
	
entity_t* entities[MAX_ENTITIES];
int entityCount = 0;

void entity_init(entity_t *entity)
{
    model_init(&entity->obj);
}


void entity_remove(entity_t *entity)
{
    model_unload(&entity->obj);
}
