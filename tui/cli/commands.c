#include "commands.h"
#include <stdio.h>
#include <string.h>

int cmd_help(int argc, char **argv) {
    (void)argc;
    (void)argv;
    printf("NeoArc commands: help, play <game>, learn, quit\n");
    return 0;
}

int cmd_play(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: play <snake|tetris|platformer>\n");
        return -1;
    }
    (void)argv;
    return 0;
}

int cmd_learn(int argc, char **argv) {
    (void)argc;
    (void)argv;
    printf("Learning module: tutorials and challenges.\n");
    return 0;
}

int cmd_quit(int argc, char **argv) {
    (void)argc;
    (void)argv;
    return 1;
}
