#include "tetris.h"
#include "renderer.h"
#include "../../../engine/input/input.h"
#include <stdlib.h>
#include <string.h>

struct tetris_s {
    int w, h;
    int score;
    int grid[32][16];
};

tetris_t *tetris_create(int w, int h) {
    tetris_t *t = (tetris_t *)malloc(sizeof(tetris_t));
    if (!t) return NULL;
    memset(t, 0, sizeof(tetris_t));
    t->w = w;
    t->h = h;
    return t;
}

void tetris_destroy(tetris_t *t) {
    free(t);
}

void tetris_tick(tetris_t *t, int key) {
    (void)t;
    (void)key;
}

int tetris_score(tetris_t *t) {
    return t ? t->score : 0;
}

int tetris_run(tetris_t *t, renderer_t *r) {
    (void)t;
    (void)r;
    return 0;
}
