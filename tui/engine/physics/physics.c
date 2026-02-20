#include "physics.h"

void physics_init(void) {
}

void physics_shutdown(void) {
}

void physics_tick(float dt) {
    (void)dt;
}

vec2_t physics_velocity(vec2_t pos, vec2_t vel, float dt) {
    vec2_t r;
    r.x = pos.x + vel.x * dt;
    r.y = pos.y + vel.y * dt;
    return r;
}
