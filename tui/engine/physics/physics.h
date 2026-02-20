#pragma once

typedef struct { float x, y; } vec2_t;

void physics_init(void);
void physics_shutdown(void);
void physics_tick(float dt);
vec2_t physics_velocity(vec2_t pos, vec2_t vel, float dt);
