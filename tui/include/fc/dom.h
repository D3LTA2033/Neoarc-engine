#pragma once
#include "types.h"
#include "buf.h"
#include "limits.h"

typedef enum {
    FC_NODE_BOX = 0,
    FC_NODE_TEXT,
    FC_NODE_PROGRESS,
} fc_node_type_t;

typedef struct fc_node fc_node_t;

struct fc_node {
    fc_node_type_t type;
    int x, y, w, h;
    bool dirty;
    bool hidden;

    fc_node_t *parent;
    fc_node_t *kids[FC_MAX_CHILDREN];
    int nkids;

    union {
        struct {
            char title[FC_STR];
            fc_align_t title_align;
            fc_border_t border;
            fc_color_t  fg;
            fc_color_t  bg;
            fc_color_t  title_fg;
        } box;
        struct {
            char text[FC_STR];
            fc_color_t fg;
            fc_color_t bg;
            fc_align_t align;
        } text;
        struct {
            float  val;
            fc_color_t on_fg;
            fc_color_t off_fg;
            bool show_pct;
        } progress;
    } d;
};

typedef struct {
    fc_buf_t  *buf;
    int w, h;
    fc_node_t nodes[FC_MAX_NODES];
    int nnodes;
} fc_dom_t;

fc_dom_t *fc_dom_new(void);
void fc_dom_free(fc_dom_t *dom);
void fc_dom_render(fc_dom_t *dom);
void fc_dom_force(fc_dom_t *dom);
void fc_dom_resize(fc_dom_t *dom, int w, int h);

fc_node_t *fc_node_new(fc_dom_t *dom, fc_node_type_t type,int x, int y, int w, int h);
void fc_node_attach(fc_node_t *parent, fc_node_t *child);
void fc_node_dirty(fc_node_t *n);
void fc_node_move(fc_node_t *n, int x, int y);
