#pragma once

#include "types.h"
#include "gen.h"


fc_buf_t *fc_buf_new(int w, int h);
void fc_buf_free(fc_buf_t *b);
void fc_buf_resize(fc_buf_t *b, int w, int h);
void fc_buf_clear(fc_buf_t *b);
void fc_buf_set(fc_buf_t *b, int x, int y, fc_cell_t c);
void fc_buf_fill(fc_buf_t *b, fc_rect_t r, fc_cell_t c);
void fc_buf_text(fc_buf_t *b, int x, int y, const char *s, fc_color_t fg, fc_color_t bg);
void fc_buf_flush(fc_buf_t *b);
void fc_buf_force(fc_buf_t *b);

static inline fc_cell_t fc_mkcell(uint32_t ch, fc_color_t fg, fc_color_t bg) {
    fc_cell_t c;
    c.ch = ch;
    c.fg = fg;
    c.bg = bg;
    c.bold = 0;
    return c;
}
