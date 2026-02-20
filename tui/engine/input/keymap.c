#include "keymap.h"
#include "input.h"
#include <string.h>

int keymap_translate(const char *seq, int len) {
    if (!seq || len <= 0) return 0;
    if (len == 1) {
        if (seq[0] == 'q' || seq[0] == 'Q') return KEY_QUIT;
        if (seq[0] == '\r' || seq[0] == '\n') return KEY_ENTER;
        return (unsigned char)seq[0];
    }
    if (len >= 3 && seq[0] == '\033' && seq[1] == '[') {
        if (seq[2] == 'A') return KEY_UP;
        if (seq[2] == 'B') return KEY_DOWN;
        if (seq[2] == 'C') return KEY_RIGHT;
        if (seq[2] == 'D') return KEY_LEFT;
    }
    return 0;
}
