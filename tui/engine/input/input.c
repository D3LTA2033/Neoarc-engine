#include "input.h"
#include "keymap.h"
#include "../frecli/include/fc/term.h"
#include <string.h>

#define KEYBUF_SIZE 32

static char s_keybuf[KEYBUF_SIZE];

void input_init(void) {
    memset(s_keybuf, 0, sizeof(s_keybuf));
}

void input_shutdown(void) {
    memset(s_keybuf, 0, sizeof(s_keybuf));
}

int input_poll(void) {
    int n = fc_term_read(s_keybuf, KEYBUF_SIZE - 1);
    if (n <= 0) return 0;
    s_keybuf[n] = '\0';
    int key = keymap_translate(s_keybuf, n);
    fc_term_drain();
    return key;
}
