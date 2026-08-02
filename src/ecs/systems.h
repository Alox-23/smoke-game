#ifndef ECS_SYSTEMS_H
#define ECS_SYSTEMS_H

#include "components.h"
#include "world.h"
#include <SDL2/SDL_render.h>

void world_movement_system(World* w, float delta_time);
void world_animation_system(World* w, float delta_time);
void world_render_system(World* w, SDL_Renderer* r);
void world_health_system(World* w);

#endif
