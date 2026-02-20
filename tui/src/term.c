#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdarg.h>


#include "fc/term.h"

#define OUTBUF (1 << 18)

static char          s_out[OUTBUF];
static int           s_len;
static struct termios s_orig;
static int           s_raw;

int fc_term_init(void) {
    if (tcgetattr(STDIN_FILENO, &s_orig) < 0) return -1;
    struct termios raw = s_orig;
    raw.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    raw.c_oflag &= ~OPOST;
    raw.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
    raw.c_cflag &= ~(CSIZE|PARENB);
    raw.c_cflag |= CS8;
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) return -1;
    s_raw = 1;
    return 0;
}

// just stop the programm
void fc_term_shutdown(void) {
    if (!s_raw) return;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &s_orig);
    s_raw = 0;
}

void fc_term_size(int *w, int *h) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col) {
        *w = ws.ws_col;
        *h = ws.ws_row;
    } else {
        *w = 80; *h = 24;
    }
}

void fc_term_write(const char *s, int len) {
    if (s_len + len >= OUTBUF) fc_term_flush();
    memcpy(s_out + s_len, s, len);
    s_len += len;
}

void fc_term_writes(const char *s) {
    fc_term_write(s, (int)strlen(s));
}

void fc_term_flush(void) {
    if (!s_len) return;
    ssize_t r = write(STDOUT_FILENO, s_out, s_len);
    (void)r;
    s_len = 0;
}

void fc_term_goto(int x, int y) {
    char buf[32];
    int n = snprintf(buf, sizeof(buf), "\033[%d;%dH", y+1, x+1);
    fc_term_write(buf, n);
}

void fc_term_clear(void) {
    fc_term_writes("\033[2J\033[H");
}

void fc_term_cursor(bool show) {
    fc_term_writes(show ? "\033[?25h" : "\033[?25l");
}

void fc_term_altscreen(bool on) {
    fc_term_writes(on ? "\033[?1049h" : "\033[?1049l");
}
