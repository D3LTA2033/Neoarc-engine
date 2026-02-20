#pragma once
#include <stddef.h>

#define BOX_THEME_ASCII_PLUS  0
#define BOX_THEME_DOUBLE     1
#define BOX_THEME_ROUND      2
#define BOX_THEME_ROUNDED    3

void box_line_top(char *out, size_t outsz, int width, int theme);
void box_line_bottom(char *out, size_t outsz, int width, int theme);
void box_line_middle(char *out, size_t outsz, int width, int theme, const char *content);
int box_content_width(const char *content);
int box_max_width(const char *title, const char **lines, int nlines);
void box_print(const char *title, const char **lines, int nlines, int theme);
