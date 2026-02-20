#pragma once

typedef struct snake_s snake_t;
typedef struct renderer_s renderer_t;

snake_t *snake_create(int w, int h);
void snake_destroy(snake_t *s);
int snake_run(snake_t *s, renderer_t *r);
void snake_tick(snake_t *s, int key);
int snake_alive(snake_t *s);
int snake_score(snake_t *s);
