#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fc/buf.h"
#include "fc/term.h"
#include "fc/colors.h"

void emit_color(fc_color_t c, int is_bg) {
    char buf[32];
    int n;
    if (c == FC_DEFAULT) {
        n = snprintf(buf, sizeof(buf), "\033[%dm", is_bg ? 49 : 39);
    } else if ((c >> 24) == 0x01) {
        n = snprintf(buf, sizeof(buf), "\033[%d;5;%um", is_bg ? 48 : 38, c & 0xFF);
    } else {
        n = snprintf(buf, sizeof(buf), "\033[%d;2;%u;%u;%um",
            is_bg ? 48 : 38, (c>>16)&0xFF, (c>>8)&0xFF, c&0xFF);
    }
    fc_term_write(buf, n);
}

int cp_utf8(uint32_t cp, char *out) {
    if (cp < 0x80)   { out[0] = cp; return 1; }
    if (cp < 0x800)  { out[0] = 0xC0|(cp>>6); out[1] = 0x80|(cp&0x3F); return 2; }
    if (cp < 0x10000){ out[0] = 0xE0|(cp>>12); out[1] = 0x80|((cp>>6)&0x3F); out[2] = 0x80|(cp&0x3F); return 3; }
    out[0]=0xF0|(cp>>18); out[1]=0x80|((cp>>12)&0x3F); out[2]=0x80|((cp>>6)&0x3F); out[3]=0x80|(cp&0x3F); return 4;
}

int utf8_decode(const char *s, uint32_t *out) {
    unsigned char c = (unsigned char)*s;
    if (!c) { *out = 0; return 0; }
    if (c < 0x80)  { *out = c; return 1; }
    if (c < 0xE0)  { *out = ((c&0x1F)<<6)|(s[1]&0x3F); return 2; }
    if (c < 0xF0)  { *out = ((c&0x0F)<<12)|((s[1]&0x3F)<<6)|(s[2]&0x3F); return 3; }
    *out = ((c&0x07)<<18)|((s[1]&0x3F)<<12)|((s[2]&0x3F)<<6)|(s[3]&0x3F); return 4;
}

fc_cell_t default_cell(void) {
    fc_cell_t c;
    c.ch = ' '; c.fg = FC_DEFAULT; c.bg = FC_DEFAULT; c.bold = 0;
    return c;
}
