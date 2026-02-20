#include "physics.h"
#include <stdlib.h>

typedef struct { int x, y, w, h; } box_t;

int collision_box_box(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    if (x1 + w1 <= x2 || x2 + w2 <= x1) return 0;
    if (y1 + h1 <= y2 || y2 + h2 <= y1) return 0;
    return 1;
}

int collision_point_box(int px, int py, int bx, int by, int bw, int bh) {
    return px >= bx && px < bx + bw && py >= by && py < by + bh;
}
