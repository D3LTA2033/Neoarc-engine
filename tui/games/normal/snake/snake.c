#include "snake.h"
#include "renderer.h"
#include "../../../engine/input/input.h"
#include "../../../engine/frecli/include/fc/term.h"
#include "../../../engine/utils/timer.h"
#include <stdlib.h>
#include <string.h>

#define MAX_SEG 256

struct snake_s {
    int w, h;
    int head_x, head_y;
    int dir_x, dir_y;
    int tail_x[MAX_SEG], tail_y[MAX_SEG];
    int tail_len;
    int food_x, food_y;
    int alive;
    int score;
};

static void place_food(snake_t *s) {
    int tries = 0;
    for (;;) {
        s->food_x = rand() % s->w;
        s->food_y = rand() % s->h;
        int hit = 0;
        if (s->head_x == s->food_x && s->head_y == s->food_y) hit = 1;
        for (int i = 0; i < s->tail_len && !hit; i++)
            if (s->tail_x[i] == s->food_x && s->tail_y[i] == s->food_y) hit = 1;
        if (!hit || ++tries > 100) break;
    }
}

snake_t *snake_create(int w, int h) {
    snake_t *s = (snake_t *)malloc(sizeof(snake_t));
    if (!s) return NULL;
    memset(s, 0, sizeof(snake_t));
    s->w = w;
    s->h = h;
    s->head_x = w / 2;
    s->head_y = h / 2;
    s->dir_x = 1;
    s->dir_y = 0;
    s->tail_len = 0;
    s->alive = 1;
    s->score = 0;
    place_food(s);
    return s;
}

void snake_destroy(snake_t *s) {
    free(s);
}

void snake_tick(snake_t *s, int key) {
    if (!s || !s->alive) return;
    if (key == KEY_UP && s->dir_y == 0) { s->dir_x = 0; s->dir_y = -1; }
    else if (key == KEY_DOWN && s->dir_y == 0) { s->dir_x = 0; s->dir_y = 1; }
    else if (key == KEY_LEFT && s->dir_x == 0) { s->dir_x = -1; s->dir_y = 0; }
    else if (key == KEY_RIGHT && s->dir_x == 0) { s->dir_x = 1; s->dir_y = 0; }
    int nx = s->head_x + s->dir_x;
    int ny = s->head_y + s->dir_y;
    if (nx < 0 || nx >= s->w || ny < 0 || ny >= s->h) { s->alive = 0; return; }
    for (int i = 0; i < s->tail_len; i++)
        if (s->tail_x[i] == nx && s->tail_y[i] == ny) { s->alive = 0; return; }
    if (s->tail_len < MAX_SEG - 1) {
        s->tail_x[s->tail_len] = s->head_x;
        s->tail_y[s->tail_len] = s->head_y;
        s->tail_len++;
    } else {
        for (int i = 0; i < s->tail_len - 1; i++) {
            s->tail_x[i] = s->tail_x[i + 1];
            s->tail_y[i] = s->tail_y[i + 1];
        }
        s->tail_x[s->tail_len - 1] = s->head_x;
        s->tail_y[s->tail_len - 1] = s->head_y;
    }
    s->head_x = nx;
    s->head_y = ny;
    if (nx == s->food_x && ny == s->food_y) {
        s->score++;
        place_food(s);
    } else if (s->tail_len > 0) {
        for (int i = 0; i < s->tail_len - 1; i++) {
            s->tail_x[i] = s->tail_x[i + 1];
            s->tail_y[i] = s->tail_y[i + 1];
        }
        s->tail_len--;
    }
}

int snake_alive(snake_t *s) {
    return s ? s->alive : 0;
}

int snake_score(snake_t *s) {
    return s ? s->score : 0;
}

static void snake_draw(snake_t *s, renderer_t *r) {
    if (!s || !r) return;
    renderer_put(r, s->head_x, s->head_y, '@');
    for (int i = 0; i < s->tail_len; i++)
        renderer_put(r, s->tail_x[i], s->tail_y[i], '*');
    renderer_put(r, s->food_x, s->food_y, '+');
}

int snake_run(snake_t *s, renderer_t *r, int fps_cap) {
    if (!s || !r) return -1;
    int frame_ms = (fps_cap > 0) ? (1000 / fps_cap) : 20;
    if (frame_ms < 1) frame_ms = 1;
    timer_init();
    double next_tick = timer_elapsed() + 0.12;
    int last_key = 0;
    while (s->alive) {
        int key = input_poll();
        if (key == KEY_QUIT || key == KEY_ESCAPE) break;
        if (key) last_key = key;
        double now = timer_elapsed();
        if (now >= next_tick) {
            snake_tick(s, last_key);
            next_tick = now + 0.12;
        }
        fc_term_clear();
        renderer_clear(r);
        snake_draw(s, r);
        renderer_put_str(r, 0, 0, "NeoArc Snake [Q]uit");
        renderer_flush(r);
        timer_sleep_ms(frame_ms);
    }
    return s->score;
}
