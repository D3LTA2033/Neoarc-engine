#include <stdlib.h>
#include <string.h>
#include "renderer.h"
#include "../../frecli/include/fc/term.h"

struct renderer_s {
    int w;
    int h;
    char *buf;
};

renderer_t *renderer_create(int w, int h) {
    renderer_t *r = (renderer_t *)malloc(sizeof(renderer_t));
    if (!r) return NULL;
    r->w = w;
    r->h = h;
    r->buf = (char *)malloc((size_t)(w * h));
    if (!r->buf) { free(r); return NULL; }
    return r;
}

void renderer_destroy(renderer_t *r) {
    if (!r) return;
    free(r->buf);
    free(r);
}

void renderer_clear(renderer_t *r) {
    if (!r || !r->buf) return;
    memset(r->buf, ' ', (size_t)(r->w * r->h));
}

void renderer_put(renderer_t *r, int x, int y, char ch) {
    if (!r || !r->buf || x < 0 || x >= r->w || y < 0 || y >= r->h) return;
    r->buf[y * r->w + x] = ch;
}

void renderer_put_str(renderer_t *r, int x, int y, const char *s) {
    if (!r || !r->buf || !s) return;
    for (; *s && x < r->w; x++, s++)
        renderer_put(r, x, y, *s);
}

void renderer_flush(renderer_t *r) {
    if (!r || !r->buf) return;
    fc_term_goto(0, 0);
    for (int y = 0; y < r->h; y++) {
        fc_term_write(r->buf + y * r->w, r->w);
        if (y < r->h - 1) fc_term_writes("\r\n");
    }
    fc_term_flush();
}

void renderer_size(renderer_t *r, int *w, int *h) {
    if (!r) return;
    if (w) *w = r->w;
    if (h) *h = r->h;
}
