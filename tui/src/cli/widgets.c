#include <string.h>
#include <stdio.h>
#include "fc/widgets.h"

/*
typedef struct {
    uint32_t tl, top, tr, ri, br, bot, bl, le;
} bchars_t;

static const bchars_t BSTYLES[] = {
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {0x250C,0x2500,0x2510,0x2502,0x2518,0x2500,0x2514,0x2502},
    {0x2554,0x2550,0x2557,0x2551,0x255D,0x2550,0x255A,0x2551},
    {0x256D,0x2500,0x256E,0x2502,0x256F,0x2500,0x2570,0x2502},
    {0x250F,0x2501,0x2513,0x2503,0x251B,0x2501,0x2517,0x2503},
    {'+','-','+','|','+','-','+','|'},
};

static int strwidth(const char *s) {
    int w = 0;
    while (*s) {
        unsigned char c = (unsigned char)*s;
        if      (c < 0x80) s += 1;
        else if (c < 0xE0) s += 2;
        else if (c < 0xF0) s += 3;
        else               s += 4;
        w++;
    }
    return w;
}
*/
