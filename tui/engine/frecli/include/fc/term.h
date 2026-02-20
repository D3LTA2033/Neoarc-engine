#pragma once
#include <stdbool.h>

int fc_term_init(void);
void fc_term_shutdown(void);
void fc_term_size(int *w, int *h);
void fc_term_clear(void);
void fc_term_flush(void);
void fc_term_goto(int x, int y);
void fc_term_write(const char *s, int len);
void fc_term_writes(const char *s);
void fc_term_cursor(bool show);
void fc_term_altscreen(bool on);
int fc_term_read(char *buf, int maxlen);
