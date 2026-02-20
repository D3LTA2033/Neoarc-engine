#include <string.h>
#include "fc/widgets.h"
#include "fc/colors.h"

fc_node_t *fc_text(fc_dom_t *dom, int x, int y, int w, int h) {
    return fc_node_new(dom, FC_NODE_TEXT, x, y, w, h);
}

void fc_text_set(fc_node_t *n, const char *s) {
    strncpy(n->d.text.text, s ? s : "", FC_STR-1);
    fc_node_dirty(n);
}

void fc_text_colors(fc_node_t *n, fc_color_t fg, fc_color_t bg) {
    n->d.text.fg = fg;
    n->d.text.bg = bg;
    fc_node_dirty(n);
}

void fc_text_align(fc_node_t *n, fc_align_t a) {
    n->d.text.align = a;
    fc_node_dirty(n);
}
