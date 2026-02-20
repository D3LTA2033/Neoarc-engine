#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fc/buf.h"
#include "fc/term.h"
#include "fc/gen.h"
#include "fc/colors.h"
#include "fc/types.h"

fc_buf_t *fc_buf_new(int w, int h) {
    fc_buf_t *b = calloc(1, sizeof(fc_buf_t));
    b->w = w; b->h = h;
    b->back  = malloc(w * h * sizeof(fc_cell_t));
    b->front = malloc(w * h * sizeof(fc_cell_t));
    fc_buf_t tmp = *b;
    (void)tmp;
    fc_cell_t def = default_cell();
    for (int i = 0; i < w*h; i++) b->back[i] = b->front[i] = def;
    b->full = true;
    return b;
}

void fc_buf_free(fc_buf_t *b) {
    if (!b) return;
    free(b->back); free(b->front); free(b);
}

void fc_buf_resize(fc_buf_t *b, int w, int h) {
    if (b->w == w && b->h == h) return;
    free(b->back); free(b->front);
    b->w = w; b->h = h;
    b->back  = malloc(w * h * sizeof(fc_cell_t));
    b->front = malloc(w * h * sizeof(fc_cell_t));
    fc_buf_clear(b);
    b->full = true;
}

void fc_buf_clear(fc_buf_t *b) {
    fc_cell_t def = default_cell();
    for (int i = 0; i < b->w * b->h; i++) b->back[i] = def;
}

void fc_buf_set(fc_buf_t *b, int x, int y, fc_cell_t c) {
    if (x < 0 || y < 0 || x >= b->w || y >= b->h) return;
    b->back[y * b->w + x] = c;
}

void fc_buf_fill(fc_buf_t *b, fc_rect_t r, fc_cell_t c) {
    for (int y = r.y; y < r.y + r.h; y++)
        for (int x = r.x; x < r.x + r.w; x++)
            fc_buf_set(b, x, y, c);
}

void fc_buf_text(fc_buf_t *b, int x, int y, const char *s, fc_color_t fg, fc_color_t bg) {
    int cx = x;
    while (*s && cx < b->w) {
        uint32_t cp;
        int n = utf8_decode(s, &cp);
        if (!n) break;
        s += n;
        if (cp == '\n') break;
        fc_buf_set(b, cx++, y, fc_mkcell(cp, fg, bg));
    }
}

void fc_buf_force(fc_buf_t *b) {
    b->full = true;
}

void fc_buf_flush(fc_buf_t *b) {
    fc_color_t cur_fg = FC_DEFAULT, cur_bg = FC_DEFAULT;
    uint8_t cur_bold = 0;
    int lx = -9999, ly = -9999;

    fc_term_writes("\033[0m");

    for (int y = 0; y < b->h; y++) {
        for (int x = 0; x < b->w; x++) {
            int i = y * b->w + x;
            fc_cell_t *bk = &b->back[i];
            fc_cell_t *fr = &b->front[i];

            bool same = bk->ch == fr->ch && bk->fg == fr->fg
                     && bk->bg == fr->bg && bk->bold == fr->bold;
            if (!b->full && same) continue;

            if (x != lx + 1 || y != ly) fc_term_goto(x, y);

            if (bk->bold != cur_bold) {
                fc_term_writes(bk->bold ? "\033[1m" : "\033[0m");
                cur_fg = cur_bg = FC_DEFAULT;
                cur_bold = bk->bold;
            }
            if (bk->fg != cur_fg) { emit_color(bk->fg, 0); cur_fg = bk->fg; }
            if (bk->bg != cur_bg) { emit_color(bk->bg, 1); cur_bg = bk->bg; }

            char tmp[5];
            uint32_t cp = bk->ch ? bk->ch : ' ';
            int n = cp_utf8(cp, tmp);
            fc_term_write(tmp, n);

            *fr = *bk;
            lx = x; ly = y;
        }
    }
    fc_term_writes("\033[0m");
    fc_term_flush();
    b->full = false;
}
