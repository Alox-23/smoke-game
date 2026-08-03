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

        if (w->positions[e].z <= 0 && w->velocities[e].dz <= 0){
            // grounded and not moving upward: snap to floor, stop falling
            w->positions[e].z = 0;
            w->velocities[e].dz = 0;
        }
        else {
            LOG_DEBUG("Velocity.z = %f", w->velocities[e].dz);
            w->velocities[e].dz -= GRAVITY * delta_time;   // see note below on this too
        }
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
        w->sprites[e].dst.y = (int)w->positions[e].y - (int)w->positions[e].z;

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
    unsigned int required = HAS_VELOCITY;
    if ((entity_mask_check(w->entity_masks[e], required))){
        return;
    }

    float magnitude = sqrtf(state->move_x * state->move_x + state->move_y * state->move_y);

    if (magnitude > 0.0001f){
        w->velocities[e].dx = state->move_x * PLAYER_SPEED / magnitude;
        w->velocities[e].dy = state->move_y * PLAYER_SPEED / magnitude;
    } else {
        w->velocities[e].dx = 0;
        w->velocities[e].dy = 0;
    }

    if (state->is_jump_pressed && w->positions[e].z == 0){
       w->velocities[e].dz = PLAYER_JUMP_STRENGTH;
       LOG_DEBUG("Player jumped");
    }
}
