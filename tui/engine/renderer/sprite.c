#include <stdlib.h>
#include <string.h>
#include "renderer.h"

typedef struct {
    int w;
    int h;
    char *data;
} sprite_t;

sprite_t *sprite_create(int w, int h) {
    sprite_t *s = (sprite_t *)malloc(sizeof(sprite_t));
    if (!s) return NULL;
    s->w = w;
    s->h = h;
    s->data = (char *)malloc((size_t)(w * h));
    if (!s->data) { free(s); return NULL; }
    memset(s->data, ' ', (size_t)(w * h));
    return s;
}

void sprite_destroy(sprite_t *s) {
    if (!s) return;
    free(s->data);
    s->data = NULL;
    free(s);
}

void sprite_set(sprite_t *s, int x, int y, char ch) {
    if (!s || !s->data || x < 0 || x >= s->w || y < 0 || y >= s->h) return;
    s->data[y * s->w + x] = ch;
}

void sprite_blit(sprite_t *s, renderer_t *r, int dx, int dy) {
    if (!s || !r) return;
    for (int y = 0; y < s->h && (dy + y) < 0; y++) (void)0;
    for (int y = 0; y < s->h; y++) {
        int ry = dy + y;
        if (ry < 0) continue;
        for (int x = 0; x < s->w; x++) {
            int rx = dx + x;
            if (rx >= 0) renderer_put(r, rx, ry, s->data[y * s->w + x]);
        }
    }
}
