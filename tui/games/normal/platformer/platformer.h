#pragma once

typedef struct platformer_s platformer_t;
typedef struct renderer_s renderer_t;

platformer_t *platformer_create(int w, int h);
void platformer_destroy(platformer_t *p);
int platformer_run(platformer_t *p, renderer_t *r);
void platformer_tick(platformer_t *p, int key);
int platformer_score(platformer_t *p);
