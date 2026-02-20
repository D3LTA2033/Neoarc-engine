#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fc/term.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

static HANDLE s_stdin;
static HANDLE s_stdout;
static DWORD s_orig_in;
static DWORD s_orig_out;
static int s_initialized;
static int s_raw;

int fc_term_init(void) {
    s_stdin = GetStdHandle(STD_INPUT_HANDLE);
    s_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (s_stdin == INVALID_HANDLE_VALUE || s_stdout == INVALID_HANDLE_VALUE) return -1;
    GetConsoleMode(s_stdin, &s_orig_in);
    GetConsoleMode(s_stdout, &s_orig_out);
    s_raw = 0;
    s_initialized = 1;
    return 0;
}

void fc_term_shutdown(void) {
    if (!s_initialized) return;
    if (s_raw) {
        SetConsoleMode(s_stdin, s_orig_in);
        SetConsoleMode(s_stdout, s_orig_out);
        s_raw = 0;
    }
    s_initialized = 0;
}

void fc_term_raw(bool on) {
    if (!s_initialized) return;
    if (on && !s_raw) {
        SetConsoleMode(s_stdin, ENABLE_WINDOW_INPUT);
        SetConsoleMode(s_stdout, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        s_raw = 1;
    } else if (!on && s_raw) {
        SetConsoleMode(s_stdin, s_orig_in);
        SetConsoleMode(s_stdout, s_orig_out);
        s_raw = 0;
    }
}

void fc_term_size(int *w, int *h) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(s_stdout, &csbi)) {
        *w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        *h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        *w = 80;
        *h = 24;
    }
}

int fc_term_read(char *buf, int maxlen) {
    DWORD n = 0;
    if (ReadConsole(s_stdin, buf, (DWORD)maxlen, &n, NULL)) return (int)n;
    return 0;
}

void fc_term_drain(void) {
    DWORD avail = 0;
    char discard[64];
    while (PeekConsoleInput(s_stdin, NULL, 0, &avail) && avail > 0)
        ReadConsole(s_stdin, discard, (DWORD)(sizeof(discard) < (size_t)avail ? sizeof(discard) : (size_t)avail), &avail, NULL);
}

#else

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#define OUTBUF (1 << 18)

static char s_out[OUTBUF];
static int s_len;
static struct termios s_orig;
static int s_raw;

int fc_term_init(void) {
    if (tcgetattr(STDIN_FILENO, &s_orig) < 0) return -1;
    s_raw = 0;
    return 0;
}

void fc_term_shutdown(void) {
    if (s_raw) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &s_orig);
        s_raw = 0;
    }
}

void fc_term_raw(bool on) {
    if (on && !s_raw) {
        struct termios raw = s_orig;
        raw.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
        raw.c_oflag &= ~OPOST;
        raw.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
        raw.c_cflag &= ~(CSIZE|PARENB);
        raw.c_cflag |= CS8;
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1;
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) return;
        s_raw = 1;
    } else if (!on && s_raw) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &s_orig);
        s_raw = 0;
    }
}

void fc_term_size(int *w, int *h) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col) {
        *w = ws.ws_col;
        *h = ws.ws_row;
    } else {
        *w = 80;
        *h = 24;
    }
}

int fc_term_read(char *buf, int maxlen) {
    ssize_t n = read(STDIN_FILENO, buf, (size_t)maxlen);
    return n >= 0 ? (int)n : 0;
}

void fc_term_drain(void) {
    fd_set fds;
    struct timeval tv = { 0, 0 };
    char c;
    for (;;) {
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) <= 0 || !FD_ISSET(STDIN_FILENO, &fds)) break;
        if (read(STDIN_FILENO, &c, 1) != 1) break;
    }
}

#endif

void fc_term_write(const char *s, int len) {
#if defined(_WIN32) || defined(_WIN64)
    DWORD written;
    if (s_stdout != INVALID_HANDLE_VALUE && len > 0)
        WriteConsole(s_stdout, s, (DWORD)len, &written, NULL);
#else
    if (s_len + len >= OUTBUF) fc_term_flush();
    memcpy(s_out + s_len, s, (size_t)len);
    s_len += len;
#endif
}

void fc_term_writes(const char *s) {
    fc_term_write(s, (int)strlen(s));
}

void fc_term_flush(void) {
#if defined(_WIN32) || defined(_WIN64)
    (void)0;
#else
    if (!s_len) return;
    write(STDOUT_FILENO, s_out, (size_t)s_len);
    s_len = 0;
#endif
}

void fc_term_goto(int x, int y) {
    char buf[32];
    int n = snprintf(buf, sizeof(buf), "\033[%d;%dH", y + 1, x + 1);
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
