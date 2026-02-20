#pragma once

typedef struct renderer_s renderer_t;

renderer_t *renderer_create(int w, int h);
void renderer_destroy(renderer_t *r);
void renderer_clear(renderer_t *r);
void renderer_put(renderer_t *r, int x, int y, char ch);
void renderer_put_str(renderer_t *r, int x, int y, const char *s);
void renderer_flush(renderer_t *r);
void renderer_size(renderer_t *r, int *w, int *h);
