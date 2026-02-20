#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fc/dom.h"
#include "fc/term.h"
#include "fc/widgets.h"
#include "fc/colors.h"

extern void render_node(fc_node_t *n, fc_buf_t *buf);

fc_dom_t *fc_dom_new(void) {
    fc_dom_t *dom = calloc(1, sizeof(fc_dom_t));
    fc_term_size(&dom->w, &dom->h);
    dom->buf = fc_buf_new(dom->w, dom->h);
    return dom;
}

void fc_dom_free(fc_dom_t *dom) {
    fc_buf_free(dom->buf);
    free(dom);
}

void fc_dom_resize(fc_dom_t *dom, int w, int h) {
    dom->w = w; dom->h = h;
    fc_buf_resize(dom->buf, w, h);
    fc_dom_force(dom);
}

fc_node_t *fc_node_new(fc_dom_t *dom, fc_node_type_t type,int x, int y, int w, int h)
{
    if (dom->nnodes >= FC_MAX_NODES) return NULL;
    fc_node_t *n = &dom->nodes[dom->nnodes++];
    memset(n, 0, sizeof(*n));
    n->type = type;
    n->x = x; n->y = y; n->w = w; n->h = h;
    n->dirty = true;
    n->d.box.fg = FC_DEFAULT;
    n->d.box.bg = FC_DEFAULT;
    n->d.box.title_fg = FC_DEFAULT;
    n->d.box.border = FC_BORDER_SINGLE;
    n->d.text.fg = FC_DEFAULT;
    n->d.text.bg = FC_DEFAULT;
    return n;
}

void fc_node_attach(fc_node_t *parent, fc_node_t *child) {
    if (parent->nkids >= FC_MAX_CHILDREN) return;
    child->parent = parent;
    parent->kids[parent->nkids++] = child;
    child->dirty = true;
}

void fc_node_dirty(fc_node_t *n) {
    n->dirty = true;
    for (int i = 0; i < n->nkids; i++) fc_node_dirty(n->kids[i]);
}

void fc_node_move(fc_node_t *n, int x, int y) {
    n->x = x; n->y = y;
    fc_node_dirty(n);
}

void fc_dom_render(fc_dom_t *dom) {
    for (int i = 0; i < dom->nnodes; i++)
        if (!dom->nodes[i].parent)
            render_node(&dom->nodes[i], dom->buf);
    fc_buf_flush(dom->buf);
}

void fc_dom_force(fc_dom_t *dom) {
    for (int i = 0; i < dom->nnodes; i++) dom->nodes[i].dirty = true;
    fc_buf_force(dom->buf);
    fc_dom_render(dom);
}
