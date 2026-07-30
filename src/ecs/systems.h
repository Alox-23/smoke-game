#ifndef ECS_SYSTEMS_H
#define ECS_SYSTEMS_H

#include "components.h"
#include "world.h"

void world_movement_system(World* w);
void world_health_system(World* w);

#endif
