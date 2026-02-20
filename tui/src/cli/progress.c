#include "fc/widgets.h"
#include "fc/colors.h"


fc_node_t *fc_progress(fc_dom_t *dom, int x, int y, int w)
{
    fc_node_t *n = fc_node_new(dom, FC_NODE_PROGRESS, x, y, w, 1);
    n->d.progress.on_fg = FC_GREEN;
    n->d.progress.off_fg = FC_GRAY;
    n->d.progress.show_pct = true;
    return n;
}
void fc_progress_set(fc_node_t *n, float val)
{
    n->d.progress.val = val < 0.0f ? 0.0f : val > 1.0f ? 1.0f : val;
    fc_node_dirty(n);
}
void fc_progress_colors(fc_node_t *n, fc_color_t on, fc_color_t off){
    n->d.progress.on_fg = on;
    n->d.progress.off_fg = off;
    fc_node_dirty(n);
}

void fc_progress_pct(fc_node_t *n, bool show) {
    n->d.progress.show_pct = show;
    fc_node_dirty(n);
}
