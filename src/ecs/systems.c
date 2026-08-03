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
        w->positions[e].x += w->velocities[e].dx * delta_time;
        w->positions[e].y += w->velocities[e].dy * delta_time;
        w->positions[e].z += w->velocities[e].dz * delta_time;
    }
}

void world_animation_system(World* w, float delta_time){
    unsigned int required = HAS_SPRITE | HAS_ANIMATION;
    for (Entity e = 0; e < (Entity)w->entity_count; e++){
        if ((entity_mask_check(w->entity_masks[e], required))) continue;

        animation_update_sate(&w->animations[e], delta_time);
        w->sprites[e].src = *animation_get_rect(&w->animations[e]);
        w->sprites[e].texture = animation_get_texture(&w->animations[e]);
    }
}

void world_render_system(World* w, SDL_Renderer* r){
    unsigned int required = HAS_POSITION | HAS_SPRITE;
    for (Entity e = 0; e < w->entity_count; e++){
        if ((entity_mask_check(w->entity_masks[e], required))) continue;
       
        w->sprites[e].dst.x = (int)w->positions[e].x;
        w->sprites[e].dst.y = (int)w->positions[e].y;

        SDL_RenderCopy(r, w->sprites[e].texture, &w->sprites[e].src, &w->sprites[e].dst);
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

void world_input_system_entity(World* w, Entity e, InputState* state){
    unsigned int requred = HAS_VELOCITY;
    if ((entity_mask_check(w->entity_masks[e], requred))){
        LOG_WARN("Entity %d does not have requred components for this entity_system");
        return;
    }

    float magnitude = sqrt(w->velocities[e].dx + w->velocities[e].dy);

    w->velocities[e].dx = state->move_x * PLAYER_SPEED / magnitude; 
    w->velocities[e].dy = state->move_y * PLAYER_SPEED / magnitude; 
}
