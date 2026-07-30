#include "systems.h"
#include "components.h"
#include "world.h"

static bool entity_mask_check(ComponentFlag a, ComponentFlag b){
    return (a & b) != b;
}

void world_movement_system(World *w, float delta_time) {
    unsigned int required = HAS_POSITION | HAS_VELOCITY;
    for (Entity e = 0; e < (Entity)w->entity_count; e++) {
        if ((entity_mask_check(w->entity_masks[e], required))) continue;
        w->positions[e].x += w->velocities[e].dx;
        w->positions[e].y += w->velocities[e].dy;
        w->positions[e].z += w->velocities[e].dz;
    }
}

void world_render_system(World* w, SDL_Renderer* r){
    unsigned int required = HAS_POSITION | HAS_TEXTURE;
    for (Entity e = 0; e < w->entity_count; e++){
        if ((entity_mask_check(w->entity_masks[e], required))) continue;
       
        LOG_DEBUG("Entity %u is being rendered", e);

        SDL_Rect dst = {
            (int)w->positions[e].x,
            (int)w->positions[e].y,
            100,
            100
        };

        SDL_RenderCopy(r, w->textures[e], NULL, &dst);
    }
}

void world_health_system(World *w) {
    unsigned int required = HAS_HEALTH;
    for (Entity e = 0; e < (Entity)w->entity_count; e++) {
        if ((entity_mask_check(w->entity_masks[e], required))) continue;
        if (w->healths[e].hp <= 0) {
            LOG_INFO("Entity %u died\n", e);
        }
    }
}
