#pragma once

#include <stdint.h>

#include "fc/buf.h"
#include "fc/term.h"
#include "fc/types.h"

typedef struct {
    uint32_t ch;
    fc_color_t fg;
    fc_color_t bg;
    uint8_t bold;
} fc_cell_t;

typedef struct {
    int w, h;
    fc_cell_t *back;
    fc_cell_t *front;
    bool full;
} fc_buf_t;


// lit just for buffer
void emit_color(fc_color_t c, int is_bg);
int cp_utf8(uint32_t cp, char *out);
int utf8_decode(const char *s, uint32_t *out);
fc_cell_t default_cell(void);
