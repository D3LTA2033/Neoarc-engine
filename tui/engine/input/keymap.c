#include "keymap.h"
#include "input.h"
#include <string.h>

static int escape_arrow(const char *seq, int len) {
    if (len < 3 || seq[1] != '[') return 0;
    int i = 2;
    while (i < len && seq[i] >= '0' && seq[i] <= '9') i++;
    while (i < len && (seq[i] == ';' || seq[i] == '?')) i++;
    if (i >= len) return 0;
    switch (seq[i]) {
        case 'A': return KEY_UP;
        case 'B': return KEY_DOWN;
        case 'C': return KEY_RIGHT;
        case 'D': return KEY_LEFT;
        default: return 0;
    }
}

int keymap_translate(const char *seq, int len) {
    if (!seq || len <= 0) return 0;
    if (seq[0] == '\033') {
        int arrow = escape_arrow(seq, len);
        if (arrow) return arrow;
        return KEY_ESCAPE;
    }
    if (len == 1) {
        if (seq[0] == 'q' || seq[0] == 'Q') return KEY_QUIT;
        if (seq[0] == '\r' || seq[0] == '\n') return KEY_ENTER;
        return (unsigned char)seq[0];
    }
    return 0;
}
