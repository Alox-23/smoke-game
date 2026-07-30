#include "world.h"

bool world_init(World* w){
    if(!w){
        LOG_ERROR("Invalid World argument");
        return false;
    }

    w->entity_count = 0;

    return true;
}

Entity world_create_entity(World* w, ComponentFlag mask){
    if (!w) {
        LOG_ERROR("Invalid World argument");
        return INVALID_ENTITY_ID;
    }
    
    Entity e;
    if (w->free_slot_count > 0){
        e = w->free_slot_list[w->free_slot_count];
    }
    else {
        e = w->entity_count++;
    }
    
    w->entity_masks[e] = mask;
    return e;
}

void world_destroy_entity(World* w, Entity e){
    w->entity_masks[e] = 0;
    w->free_slot_list[w->free_slot_count++] = e;
}

void world_shutdown(World* w){
    if (!w){
        LOG_ERROR("Invalid World argument");
        return;
    }

    //free world
}
