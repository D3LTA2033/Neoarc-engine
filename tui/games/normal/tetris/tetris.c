#include "tetris.h"
#include "renderer.h"
#include "../../../engine/input/input.h"
#include "../../../engine/frecli/include/fc/term.h"
#include "../../../engine/utils/timer.h"
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

int tetris_run(tetris_t *t, renderer_t *r, int fps_cap) {
    if (!t || !r) return 0;
    int frame_ms = (fps_cap > 0) ? (1000 / fps_cap) : 50;
    if (frame_ms < 1) frame_ms = 1;
    timer_init();
    for (;;) {
        int key = input_poll();
        if (key == KEY_QUIT || key == KEY_ESCAPE) break;
        tetris_tick(t, key);
        fc_term_clear();
        renderer_clear(r);
        renderer_put_str(r, 0, 0, "NeoArc Tetris");
        renderer_put_str(r, 0, 1, "Press Q or ESC to quit");
        renderer_flush(r);
        timer_sleep_ms(frame_ms);
    }
    return t->score;
}
