#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef uint32_t fc_color_t;

typedef enum {
    FC_BORDER_NONE = 0,
    FC_BORDER_SINGLE,
    FC_BORDER_DOUBLE,
    FC_BORDER_ROUNDED,
    FC_BORDER_THICK,
    FC_BORDER_ASCII,
} fc_border_t;

typedef enum {
    FC_ALIGN_LEFT = 0,
    FC_ALIGN_CENTER,
    FC_ALIGN_RIGHT,
} fc_align_t;

typedef struct { int x, y, w, h; } fc_rect_t;
