#pragma once

typedef struct tetris_s tetris_t;
typedef struct renderer_s renderer_t;

tetris_t *tetris_create(int w, int h);
void tetris_destroy(tetris_t *t);
int tetris_run(tetris_t *t, renderer_t *r, int fps_cap);
void tetris_tick(tetris_t *t, int key);
int tetris_score(tetris_t *t);
