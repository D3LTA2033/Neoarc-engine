#include "neoarc.h"
#include "menu.h"
#include "commands.h"
#include "../engine/frecli/include/fc/term.h"
#include "../engine/renderer/renderer.h"
#include "../games/normal/snake/snake.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    if (fc_term_init() < 0) {
        fprintf(stderr, "NeoArc: terminal init failed\n");
        return 1;
    }
    fc_term_cursor(1);
    fc_term_altscreen(0);
    input_init();
    int w = 80, h = 24;
    fc_term_size(&w, &h);
    if (argc >= 2 && strcmp(argv[1], "snake") == 0) {
        renderer_t *r = renderer_create(w, h);
        if (!r) { fc_term_shutdown(); return 1; }
        snake_t *snake = snake_create(w, h);
        if (!snake) { renderer_destroy(r); fc_term_shutdown(); return 1; }
        fc_term_clear();
        fc_term_cursor(0);
        fc_term_altscreen(1);
        int score = snake_run(snake, r);
        fc_term_altscreen(0);
        fc_term_cursor(1);
        snake_destroy(snake);
        renderer_destroy(r);
        printf("Score: %d\n", score);
    } else {
        menu_show();
        while (menu_run() == 0)
            menu_show();
    }
    input_shutdown();
    fc_term_shutdown();
    return 0;
}
