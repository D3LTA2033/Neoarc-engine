#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "types.h"

// terminal default color
#define FC_DEFAULT 0xFFFFFFFFu
// 256 color palette
#define FC_C256(n) (0x01000000u | (uint32_t)(n))
// truecolor
#define FC_RGB(r,g,b) (0x02000000u | ((uint32_t)(r)<<16) | ((uint32_t)(g)<<8) | (uint32_t)(b))

#define FC_RED         FC_C256(1)
#define FC_GREEN       FC_C256(2)
#define FC_YELLOW      FC_C256(3)
#define FC_BLUE        FC_C256(4)
#define FC_MAGENTA     FC_C256(5)
#define FC_CYAN        FC_C256(6)
#define FC_WHITE       FC_C256(7)
#define FC_GRAY        FC_C256(8)
#define FC_BRIGHT_RED  FC_C256(9)
