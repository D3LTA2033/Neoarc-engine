#pragma once
#include "dom.h"

fc_node_t *fc_box(fc_dom_t *dom, int x, int y, int w, int h);
void fc_box_title(fc_node_t *n, const char *title);
void fc_box_title_align(fc_node_t *n, fc_align_t a);
void fc_box_border(fc_node_t *n, fc_border_t b);
void fc_box_colors(fc_node_t *n, fc_color_t fg, fc_color_t bg);
void fc_box_title_color(fc_node_t *n, fc_color_t fg);

fc_node_t *fc_text(fc_dom_t *dom, int x, int y, int w, int h);
void fc_text_set(fc_node_t *n, const char *s);
void fc_text_colors(fc_node_t *n, fc_color_t fg, fc_color_t bg);
void fc_text_align(fc_node_t *n, fc_align_t a);

fc_node_t *fc_progress(fc_dom_t *dom, int x, int y, int w);
void fc_progress_set(fc_node_t *n, float val);
void fc_progress_colors(fc_node_t *n, fc_color_t on, fc_color_t off);
void fc_progress_pct(fc_node_t *n, bool show);

void fc_draw_box(fc_buf_t *buf, int x, int y, int w, int h, const char *title, fc_align_t talign,fc_border_t border, fc_color_t fg, fc_color_t bg, fc_color_t title_fg);
