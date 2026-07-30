#ifndef ECS_WORLD_H
#define ECS_WORLD_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "components.h"
#include "../core/log.h"

#define MAX_ENTITIES 256
#define INVALID_ENTITY_ID UINT32_MAX

typedef uint32_t Entity;

typedef struct {
    Position positions[MAX_ENTITIES];
    Velocity velocities[MAX_ENTITIES];
    Health healths[MAX_ENTITIES];

    Entity free_slot_list[MAX_ENTITIES]; 
    ComponentFlag entity_masks[MAX_ENTITIES];

    Entity free_slot_count;
    Entity entity_count;
} World;

bool world_init(World* w);
Entity world_create_entity(World* w, ComponentFlag mask);
void world_destroy_entity(World* w, Entity e);
void world_shutdown(World* w);

#endif
