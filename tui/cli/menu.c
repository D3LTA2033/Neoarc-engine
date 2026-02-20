#include "menu.h"
#include "commands.h"
#include "config.h"
#include "box.h"
#include "neoarc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define GAMES_COUNT 3
static const char *games_name[] = { "Snake", "Tetris", "Platformer" };

static int box_theme(void) {
    return config_get_box_theme();
}

static int str_contains(const char *hay, const char *needle) {
    const char *p = hay;
    int nlen = (int)strlen(needle);
    if (nlen == 0) return 1;
    while (*p) {
        if (tolower((unsigned char)*p) == tolower((unsigned char)needle[0])) {
            int i = 1;
            while (i < nlen && p[i] && tolower((unsigned char)p[i]) == tolower((unsigned char)needle[i])) i++;
            if (i == nlen) return 1;
        }
        p++;
    }
    return 0;
}

void menu_show(void) {
    const char *title = "  NeoArc  ";
    const char *lines[] = {
        "  1. Search games",
        "  2. Play",
        "  3. Learn",
        "  4. Help",
        "  5. Settings",
        "  6. Credits",
        "  7. Quit",
        NULL
    };
    int n = 0;
    while (lines[n]) n++;
    box_print(title, lines, n, box_theme());
    printf("\n  %s\n", NEOARC_VERSION);
    printf("  Choice: ");
    fflush(stdout);
}

int menu_run(void) {
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) return MENU_QUIT;
    int n = atoi(buf);
    if (n >= 1 && n <= 7) return n;
    return 0;
}

int menu_play_submenu(void) {
    const char *title = "  Play  ";
    const char *lines[] = {
        "  1. Snake",
        "  2. Tetris",
        "  3. Platformer",
        "  0. Back",
        NULL
    };
    int n = 0;
    while (lines[n]) n++;
    for (;;) {
        box_print(title, lines, n, box_theme());
        printf("  Choice: ");
        fflush(stdout);
        char buf[64];
        if (!fgets(buf, sizeof(buf), stdin)) return 0;
        int c = atoi(buf);
        if (c == 0) return 0;
        if (c >= 1 && c <= 3) return c;
    }
}

void menu_search(int *out_game_index) {
    *out_game_index = 0;
    const char *title = "  Search games  ";
    const char *hint[] = { "  Games from: github.com/D3LTA2033/Neoarc-engine  ", NULL };
    box_print(title, hint, 1, box_theme());
    printf("  Search: ");
    fflush(stdout);
    char query[128];
    if (!fgets(query, sizeof(query), stdin)) return;
    query[strcspn(query, "\r\n")] = '\0';
    int match[GAMES_COUNT];
    int nmatch = 0;
    if (query[0] == '\0') {
        for (int i = 0; i < GAMES_COUNT; i++) match[nmatch++] = i;
    } else {
        for (int i = 0; i < GAMES_COUNT; i++)
            if (str_contains(games_name[i], query)) match[nmatch++] = i;
    }
    if (nmatch == 0) {
        const char *msg[] = { "  No matches.  ", NULL };
        box_print("  Results  ", msg, 1, box_theme());
        return;
    }
    char res[16][64];
    const char *ptrs[16];
    for (int i = 0; i < nmatch && i < 16; i++) {
        snprintf(res[i], sizeof(res[i]), "  %d. %s  ", i + 1, games_name[match[i]]);
        ptrs[i] = res[i];
    }
    ptrs[nmatch] = NULL;
    box_print("  Results  ", ptrs, nmatch, box_theme());
    printf("  Pick (0=back): ");
    fflush(stdout);
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) return;
    int pick = atoi(buf);
    if (pick >= 1 && pick <= nmatch) *out_game_index = match[pick - 1] + 1;
}

void menu_settings(void) {
    const char *title = "  Settings  ";
    for (;;) {
        char fps_line[64];
        int fps = config_get_fps_cap();
        const char *fps_str = fps == 0 ? "Uncapped" : (fps == 30 ? "30" : fps == 60 ? "60" : fps == 120 ? "120" : "Custom");
        snprintf(fps_line, sizeof(fps_line), "  FPS cap: %s  ", fps_str);
        const char *theme_names[] = { "ASCII", "Double", "Round", "Rounded" };
        char theme_line[64];
        snprintf(theme_line, sizeof(theme_line), "  Box theme: %s  ", theme_names[config_get_box_theme()]);
        const char *lines[] = {
            "  1. Set FPS cap (30/60/120/0=Uncapped)",
            "  2. Set box theme (1-4)",
            "  0. Back",
            NULL
        };
        int n = 0;
        while (lines[n]) n++;
        box_print(title, lines, n, box_theme());
        printf("  Choice: ");
        fflush(stdout);
        char buf[64];
        if (!fgets(buf, sizeof(buf), stdin)) return;
        int c = atoi(buf);
        if (c == 0) return;
        if (c == 1) {
            printf("  FPS (30/60/120/0): ");
            fflush(stdout);
            if (fgets(buf, sizeof(buf), stdin)) {
                int v = atoi(buf);
                config_set_fps_cap(v);
                config_save();
            }
        } else if (c == 2) {
            printf("  Theme 1=ASCII 2=Double 3=Round 4=Rounded: ");
            fflush(stdout);
            if (fgets(buf, sizeof(buf), stdin)) {
                int t = atoi(buf);
                if (t >= 1 && t <= 4) {
                    config_set_box_theme(t - 1);
                    config_save();
                }
            }
        }
    }
}

void menu_credits(void) {
    const char *title = "  Credits  ";
    const char *lines[] = {
        "  @emexos  ",
        "  Co-Founder & Main dev. (backend & frontend)  ",
        "  Created TUI, frecli.  ",
        "  ",
        "  @mcs.s  ",
        "  Founder & Main dev. (backend & frontend)  ",
        "  Created full backend & systems.  ",
        NULL
    };
    int n = 0;
    while (lines[n]) n++;
    box_print(title, lines, n, box_theme());
    printf("  Press Enter... ");
    fflush(stdout);
    char buf[8];
    fgets(buf, sizeof(buf), stdin);
}
