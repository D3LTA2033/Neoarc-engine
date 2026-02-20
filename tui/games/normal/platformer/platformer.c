#include "platformer.h"
#include "renderer.h"
#include "../../../engine/input/input.h"
#include "../../../engine/frecli/include/fc/term.h"
#include "../../../engine/utils/timer.h"
#include <stdlib.h>
#include <string.h>

struct platformer_s {
    int w, h;
    int score;
    int player_x, player_y;
};

platformer_t *platformer_create(int w, int h) {
    platformer_t *p = (platformer_t *)malloc(sizeof(platformer_t));
    if (!p) return NULL;
    memset(p, 0, sizeof(platformer_t));
    p->w = w;
    p->h = h;
    p->player_x = 2;
    p->player_y = h - 2;
    return p;
}

void platformer_destroy(platformer_t *p) {
    free(p);
}

void platformer_tick(platformer_t *p, int key) {
    (void)key;
    if (!p) return;
    (void)p->player_x;
    (void)p->player_y;
}

int platformer_score(platformer_t *p) {
    return p ? p->score : 0;
}

int platformer_run(platformer_t *p, renderer_t *r, int fps_cap) {
    if (!p || !r) return 0;
    int frame_ms = (fps_cap > 0) ? (1000 / fps_cap) : 50;
    if (frame_ms < 1) frame_ms = 1;
    timer_init();
    for (;;) {
        int key = input_poll();
        if (key == KEY_QUIT || key == KEY_ESCAPE) break;
        platformer_tick(p, key);
        fc_term_clear();
        renderer_clear(r);
        renderer_put(r, p->player_x, p->player_y, 'P');
        renderer_put_str(r, 0, 0, "NeoArc Platformer [Q]uit");
        renderer_flush(r);
        timer_sleep_ms(frame_ms);
    }
    return p->score;
}
