#include "input.h"
#include "keymap.h"
#include "../../frecli/include/fc/term.h"
#include <string.h>

static char s_keybuf[32];
static int s_keylen;

void input_init(void) {
    s_keylen = 0;
    memset(s_keybuf, 0, sizeof(s_keybuf));
}

void input_shutdown(void) {
    (void)0;
}

int input_poll(void) {
    int n = fc_term_read(s_keybuf, (int)(sizeof(s_keybuf) - 1));
    if (n <= 0) return 0;
    s_keybuf[n] = '\0';
    s_keylen = n;
    return keymap_translate(s_keybuf, n);
}
