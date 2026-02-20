#include "neoarc.h"
#include "menu.h"
#include "commands.h"
#include "config.h"
#include "../engine/frecli/include/fc/term.h"
#include "../engine/renderer/renderer.h"
#include "../engine/input/input.h"
#include "../games/normal/snake/snake.h"
#include "../games/normal/tetris/tetris.h"
#include "../games/normal/platformer/platformer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MENU_QUIT 7

static int run_snake(int w, int h) {
    renderer_t *r = renderer_create(w, h);
    if (!r) return -1;
    snake_t *snake = snake_create(w, h);
    if (!snake) { renderer_destroy(r); return -1; }
    fc_term_raw(1);
    fc_term_clear();
    fc_term_cursor(0);
    fc_term_altscreen(1);
    int score = snake_run(snake, r, config_get_fps_cap());
    fc_term_altscreen(0);
    fc_term_cursor(1);
    fc_term_raw(0);
    snake_destroy(snake);
    renderer_destroy(r);
    return score;
}

static int run_tetris(int w, int h) {
    renderer_t *r = renderer_create(w, h);
    if (!r) return -1;
    tetris_t *t = tetris_create(w, h);
    if (!t) { renderer_destroy(r); return -1; }
    fc_term_raw(1);
    fc_term_clear();
    fc_term_cursor(0);
    fc_term_altscreen(1);
    int score = tetris_run(t, r, config_get_fps_cap());
    fc_term_altscreen(0);
    fc_term_cursor(1);
    fc_term_raw(0);
    tetris_destroy(t);
    renderer_destroy(r);
    return score;
}

static int run_platformer(int w, int h) {
    renderer_t *r = renderer_create(w, h);
    if (!r) return -1;
    platformer_t *p = platformer_create(w, h);
    if (!p) { renderer_destroy(r); return -1; }
    fc_term_raw(1);
    fc_term_clear();
    fc_term_cursor(0);
    fc_term_altscreen(1);
    int score = platformer_run(p, r, config_get_fps_cap());
    fc_term_altscreen(0);
    fc_term_cursor(1);
    fc_term_raw(0);
    platformer_destroy(p);
    renderer_destroy(r);
    return score;
}

static void run_game_by_index(int index, int w, int h) {
    if (index == 1) run_snake(w, h);
    else if (index == 2) run_tetris(w, h);
    else if (index == 3) run_platformer(w, h);
}

int main(int argc, char **argv) {
    if (fc_term_init() < 0) {
        fprintf(stderr, "NeoArc: terminal init failed\n");
        return 1;
    }
    fc_term_cursor(1);
    fc_term_altscreen(0);
    input_init();
    config_load();
    int w = 80, h = 24;
    fc_term_size(&w, &h);
    if (argc >= 2) {
        int score = -1;
        if (strcmp(argv[1], "snake") == 0) score = run_snake(w, h);
        else if (strcmp(argv[1], "tetris") == 0) score = run_tetris(w, h);
        else if (strcmp(argv[1], "platformer") == 0) score = run_platformer(w, h);
        if (score >= 0) {
            printf("Score: %d\n", score);
            input_shutdown();
            fc_term_shutdown();
            return 0;
        }
    }
    for (;;) {
        menu_show();
        int choice = menu_run();
        if (choice == MENU_QUIT) break;
        if (choice == 1) {
            int game_index = 0;
            menu_search(&game_index);
            if (game_index) run_game_by_index(game_index, w, h);
            continue;
        }
        if (choice == 2) {
            int game_index = menu_play_submenu();
            if (game_index) run_game_by_index(game_index, w, h);
            continue;
        }
        if (choice == 3) { cmd_learn(0, NULL); continue; }
        if (choice == 4) { cmd_help(0, NULL); continue; }
        if (choice == 5) { menu_settings(); continue; }
        if (choice == 6) { menu_credits(); continue; }
    }
    input_shutdown();
    fc_term_shutdown();
    return 0;
}
