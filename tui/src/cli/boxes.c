#include <string.h>
#include "fc/widgets.h"
#include "fc/colors.h"
#include "border.h"

void fc_draw_box(fc_buf_t *buf, int x, int y, int w, int h, const char *title, fc_align_t talign,
fc_border_t border, fc_color_t fg, fc_color_t bg, fc_color_t title_fg) // its not enough space.... :(
{
    if (w < 2 || h < 2) return;
    const bchars_t *b = get_border(border);

    fc_buf_fill(buf, (fc_rect_t){x, y, w, h}, fc_mkcell(' ', fg, bg));

    fc_buf_set(buf, x,      y,     fc_mkcell(b->tl, fg, bg));
    fc_buf_set(buf, x+w-1,  y,     fc_mkcell(b->tr, fg, bg));
    fc_buf_set(buf, x,      y+h-1, fc_mkcell(b->bl, fg, bg));
    fc_buf_set(buf, x+w-1,  y+h-1, fc_mkcell(b->br, fg, bg));

    for (int i = 1; i < w-1; i++) {
        fc_buf_set(buf, x+i, y,     fc_mkcell(b->top, fg, bg));
        fc_buf_set(buf, x+i, y+h-1, fc_mkcell(b->bot, fg, bg));
    }
    for (int i = 1; i < h-1; i++) {
        fc_buf_set(buf, x,     y+i, fc_mkcell(b->le, fg, bg));
        fc_buf_set(buf, x+w-1, y+i, fc_mkcell(b->ri, fg, bg));
    }

    if (!title || !*title) return;

    int tw = strwidth(title);
    int inner = w - 4;
    if (tw > inner) tw = inner;

    int tx;
    if      (talign == FC_ALIGN_CENTER) tx = x + (w - tw) / 2;
    else if (talign == FC_ALIGN_RIGHT)  tx = x + w - tw - 2;
    else                                tx = x + 2;

    fc_buf_set(buf, tx-1, y, fc_mkcell(' ', fg, bg));

    const char *p = title;
    int cx = tx;
    while (*p && cx < x+w-1) {
        uint32_t cp = (unsigned char)*p;
        int bytes = 1;
        if      (cp >= 0xF0) { bytes=4; cp=((cp&0x07)<<18)|((p[1]&0x3F)<<12)|((p[2]&0x3F)<<6)|(p[3]&0x3F); }
        else if (cp >= 0xE0) { bytes=3; cp=((cp&0x0F)<<12)|((p[1]&0x3F)<<6)|(p[2]&0x3F); }
        else if (cp >= 0xC0) { bytes=2; cp=((cp&0x1F)<<6)|(p[1]&0x3F); }
        p += bytes;
        fc_buf_set(buf, cx++, y, fc_mkcell(cp, title_fg, bg));
    }
    fc_buf_set(buf, cx, y, fc_mkcell(' ', fg, bg));
}

fc_node_t *fc_box(fc_dom_t *dom, int x, int y, int w, int h) {
    return fc_node_new(dom, FC_NODE_BOX, x, y, w, h);
}

void fc_box_title(fc_node_t *n, const char *t) {
    strncpy(n->d.box.title, t ? t : "", FC_STR-1);
    fc_node_dirty(n);
}

void fc_box_title_align(fc_node_t *n, fc_align_t a) {
    n->d.box.title_align = a;
    fc_node_dirty(n);
}

void fc_box_border(fc_node_t *n, fc_border_t b) {
    n->d.box.border = b;
    fc_node_dirty(n);
}

void fc_box_colors(fc_node_t *n, fc_color_t fg, fc_color_t bg) {
    n->d.box.fg = fg;
    n->d.box.bg = bg;
    fc_node_dirty(n);
}

void fc_box_title_color(fc_node_t *n, fc_color_t fg) {
    n->d.box.title_fg = fg;
    fc_node_dirty(n);
}
