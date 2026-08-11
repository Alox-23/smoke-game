#include "systems.h"
#include "components.h"
#include "world.h"

static bool entity_mask_check(ComponentFlag a, ComponentFlag b){
    return (a & b) != b;
}

void world_movement_system(World *w, float delta_time) {
    ComponentFlag required = HAS_POSITION | HAS_VELOCITY;
    for (Entity e = 0; e < (Entity)w->entity_count; e++) {
        if (entity_mask_check(w->entity_masks[e], required)) continue;
        w->positions[e].x += w->velocities[e].x * delta_time;
        w->positions[e].y += w->velocities[e].y * delta_time;
        
        if (w->positions[e].z <= 0 && w->velocities[e].z <= 0){
            // grounded and not moving upward: snap to floor, stop falling
            w->positions[e].z = 0;
            w->velocities[e].z = 0;
        }
        else {
            w->velocities[e].z -= GRAVITY * delta_time;   // see note below on this too
        }
        w->positions[e].z += w->velocities[e].z * delta_time;    
    }
}

void world_animation_system(World* w, float delta_time){
    ComponentFlag required = HAS_SPRITE | HAS_ANIMATION;
    for (Entity e = 0; e < (Entity)w->entity_count; e++){
        if (entity_mask_check(w->entity_masks[e], required)) continue;
       
        Vector3 v = w->velocities[e];

        if (v.x > 0) animation_play_clip(&w->animations[e], animation_get_id_by_name(&w->animations[e], "walk_right"));
        if (v.x < 0) animation_play_clip(&w->animations[e], animation_get_id_by_name(&w->animations[e], "walk_left"));
        if (v.y > 0) animation_play_clip(&w->animations[e], animation_get_id_by_name(&w->animations[e], "walk_down"));
        if (v.y < 0) animation_play_clip(&w->animations[e], animation_get_id_by_name(&w->animations[e], "walk_up"));

        if (v.x != 0 || v.y != 0 || v.z != 0) animation_update_sate(&w->animations[e], delta_time);
        w->sprites[e].src = *animation_get_rect(&w->animations[e]);
        w->sprites[e].texture = animation_get_texture(&w->animations[e]);
    }
}

void world_render_system(World* w, SDL_Renderer* r){
    ComponentFlag required = HAS_POSITION | HAS_SPRITE;
    for (Entity e = 0; e < w->entity_count; e++){
        if (entity_mask_check(w->entity_masks[e], required)) continue;

        w->sprites[e].dst.x = w->positions[e].x + w->scroll_offset.x;
        w->sprites[e].dst.y = w->positions[e].y - w->positions[e].z + w-> scroll_offset.y;

        LOG_DEBUG("{%d, %d}, {%d, %d}, {%3.f, %3.f}", w->sprites[e].src.x, w->sprites[e].src.y, w->sprites[e].dst.x, w->sprites[e].dst.y, w->positions[e].x, w->positions[e].y);

        SDL_Rect dst_centered = {
            w->sprites[e].dst.x - w->sprites[e].dst.w / 2,
            w->sprites[e].dst.y - w->sprites[e].dst.h / 2,
            w->sprites[e].dst.w,
            w->sprites[e].dst.h,
        };
        
        SDL_RenderCopy(r, w->sprites[e].texture, &w->sprites[e].src, &dst_centered);
    }
}

void world_health_system(World *w) {
    ComponentFlag required = HAS_HEALTH;
    for (Entity e = 0; e < (Entity)w->entity_count; e++) {
        if (entity_mask_check(w->entity_masks[e], required)) continue;
        if (w->healths[e].hp <= 0) {
            LOG_INFO("Entity %u died\n", e);
        }
    }
}

void world_scroll_system(World* w, Entity ref, float delta_time){
    if (!w){
        LOG_ERROR("Invalid World argument");
        return;
    }

    if (entity_mask_check(w->entity_masks[ref], HAS_VELOCITY)){
        LOG_WARN("Reference Entity does not have velocity component");
        return;
    }

    w->scroll_offset.x -= w->velocities[ref].x * delta_time;
    w->scroll_offset.y -= w->velocities[ref].y * delta_time;
}

void world_input_system_entity(World* w, Entity e, InputState* state){
    ComponentFlag required = HAS_VELOCITY;
    if (entity_mask_check(w->entity_masks[e], required)){
        return;
    }

    float magnitude = sqrtf(state->move_x * state->move_x + state->move_y * state->move_y);

    if (magnitude > 0.0001f){
        w->velocities[e].x = state->move_x * PLAYER_SPEED / magnitude;
        w->velocities[e].y = state->move_y * PLAYER_SPEED / magnitude;
    } else {
        w->velocities[e].x = 0;
        w->velocities[e].y = 0;
    }

    if (state->is_jump_pressed && w->positions[e].z == 0){
       w->velocities[e].z = PLAYER_JUMP_STRENGTH;
    }
}
