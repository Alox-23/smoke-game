#include "systems.h"

void movement_system(World *w) {
    unsigned int required = HAS_POSITION | HAS_VELOCITY;
    for (Entity e = 0; e < (Entity)w->entity_count; e++) {
        if ((w->entity_masks[e] & required) != required) continue;
        w->positions[e].x += w->velocities[e].dx;
        w->positions[e].y += w->velocities[e].dy;
        w->positions[e].z += w->velocities[e].dz;
    }
}
 
void health_system(World *w) {
    for (Entity e = 0; e < (Entity)w->entity_count; e++) {
        if (!(w->entity_masks[e] & HAS_HEALTH)) continue;
        if (w->healths[e].hp <= 0) {
            LOG_INFO("Entity %u died\n", e);
        }
    }
}
