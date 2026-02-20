#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fc/dom.h"
#include "fc/term.h"
#include "fc/widgets.h"
#include "fc/colors.h"

void render_node(fc_node_t *n, fc_buf_t *buf)
{
    if (n->hidden) return;
    if (n->dirty) {
        switch (n->type) {
            case FC_NODE_BOX:
                fc_draw_box(buf, n->x, n->y, n->w, n->h,
                    n->d.box.title, n->d.box.title_align,
                    n->d.box.border, n->d.box.fg, n->d.box.bg, n->d.box.title_fg);
                break;
            case FC_NODE_TEXT: {
                fc_rect_t r = {n->x, n->y, n->w, n->h};
                fc_cell_t fill = fc_mkcell(' ', n->d.text.fg, n->d.text.bg);
                fc_buf_fill(buf, r, fill);
                const char *s = n->d.text.text;
                int row = 0;
                while (*s && row < n->h) {
                    const char *line = s;
                    int cols = 0;
                    while (*s && *s != '\n') { s++; cols++; }
                    if (*s == '\n') s++;
                    int tx = n->x;
                    if (n->d.text.align == FC_ALIGN_CENTER) tx += (n->w - cols) / 2;
                    else if (n->d.text.align == FC_ALIGN_RIGHT) tx += n->w - cols;
                    char tmp[FC_STR];
                    int copy = cols < FC_STR-1 ? cols : FC_STR-1;
                    memcpy(tmp, line, copy); tmp[copy] = 0;
                    fc_buf_text(buf, tx, n->y + row, tmp, n->d.text.fg, n->d.text.bg);
                    row++;
                }
                break;
            }
            case FC_NODE_PROGRESS: {
                int filled = (int)(n->d.progress.val * n->w);
                for (int i = 0; i < n->w; i++) {
                    fc_color_t fg = i < filled ? n->d.progress.on_fg : n->d.progress.off_fg;
                    uint32_t ch   = i < filled ? 0x2588 : 0x2591;
                    fc_buf_set(buf, n->x + i, n->y, fc_mkcell(ch, fg, FC_DEFAULT));
                }
                if (n->d.progress.show_pct) {
                    char pct[8];
                    snprintf(pct, sizeof(pct), "%3d%%", (int)(n->d.progress.val * 100));
                    int px = n->x + (n->w - 4) / 2;
                    fc_buf_text(buf, px, n->y, pct, FC_DEFAULT, FC_DEFAULT);
                }
                break;
            }
        }
        n->dirty = false;
    }
    for (int i = 0; i < n->nkids; i++) render_node(n->kids[i], buf);
}
