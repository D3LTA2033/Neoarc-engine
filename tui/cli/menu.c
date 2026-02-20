#include "menu.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *items[] = { "Play", "Learn", "Help", "Quit", NULL };

void menu_show(void) {
    printf("=== NeoArc ===\n");
    for (int i = 0; items[i]; i++)
        printf("  %d. %s\n", i + 1, items[i]);
}

int menu_run(void) {
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    int n = atoi(buf);
    if (n == 1) return cmd_play(0, NULL);
    if (n == 2) return cmd_learn(0, NULL);
    if (n == 3) return cmd_help(0, NULL);
    if (n == 4) return cmd_quit(0, NULL);
    return 0;
}
