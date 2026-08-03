#ifndef ECS_SYSTEMS_H
#define ECS_SYSTEMS_H

#include "components.h"
#include "world.h"
#include "../core/input.h"

#include <SDL2/SDL_render.h>

void world_movement_system(World* w, float delta_time);
void world_render_system(World* w, SDL_Renderer* r);
void world_health_system(World* w);

//per-entity systems
void world_input_system_entity(World* w, Entity e, InputState* state);

#endif
