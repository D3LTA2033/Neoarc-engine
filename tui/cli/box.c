#include "box.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *theme_tl[] = { "+", "\xE2\x95\x94", "\xE2\x94\x8C", "\xE2\x95\xAD" };
static const char *theme_tr[] = { "+", "\xE2\x95\x97", "\xE2\x94\x90", "\xE2\x95\xAE" };
static const char *theme_bl[] = { "+", "\xE2\x95\x9A", "\xE2\x94\x94", "\xE2\x95\xB0" };
static const char *theme_br[] = { "+", "\xE2\x95\x9D", "\xE2\x94\x98", "\xE2\x95\xAF" };
static const char *theme_h[]  = { "-", "\xE2\x95\x90", "\xE2\x94\x80", "\xE2\x94\x80" };
static const char *theme_v[]  = { "|", "\xE2\x95\x91", "\xE2\x94\x82", "\xE2\x94\x82" };

static int utf8_len(const char *s) {
    int n = 0;
    while (*s) {
        unsigned char c = (unsigned char)*s;
        if (c < 0x80) { s++; n++; }
        else if ((c & 0xE0) == 0xC0) { s += 2; n++; }
        else if ((c & 0xF0) == 0xE0) { s += 3; n++; }
        else if ((c & 0xF8) == 0xF0) { s += 4; n++; }
        else { s++; n++; }
    }
    return n;
}

static void utf8_pad(char *out, size_t outsz, const char *s, int width) {
    int len = utf8_len(s);
    size_t j = 0;
    if (len >= width) {
        while (width > 0 && *s) {
            unsigned char c = (unsigned char)*s;
            int step = 1;
            if (c >= 0xF0) step = 4;
            else if (c >= 0xE0) step = 3;
            else if (c >= 0xC0) step = 2;
            if (j + step < outsz) {
                for (int i = 0; i < step; i++) out[j++] = s[i];
            }
            s += step;
            width--;
        }
        out[j] = '\0';
        return;
    }
    j = (size_t)snprintf(out, outsz, "%s", s);
    for (int i = len; i < width && j + 2 < outsz; i++) { out[j++] = ' '; out[j] = '\0'; }
}

int box_content_width(const char *content) {
    return content ? utf8_len(content) : 0;
}

int box_max_width(const char *title, const char **lines, int nlines) {
    int w = title ? utf8_len(title) : 0;
    for (int i = 0; i < nlines && lines; i++)
        if (lines[i]) {
            int l = utf8_len(lines[i]);
            if (l > w) w = l;
        }
    return w;
}

void box_line_top(char *out, size_t outsz, int width, int theme) {
    int t = (theme >= 0 && theme <= 3) ? theme : 0;
    size_t j = 0;
    j += (size_t)snprintf(out + j, outsz - j, "%s", theme_tl[t]);
    for (int i = 0; i < width && j < outsz - 4; i++) j += (size_t)snprintf(out + j, outsz - j, "%s", theme_h[t]);
    snprintf(out + j, outsz - j, "%s", theme_tr[t]);
}

void box_line_bottom(char *out, size_t outsz, int width, int theme) {
    int t = (theme >= 0 && theme <= 3) ? theme : 0;
    size_t j = 0;
    j += (size_t)snprintf(out + j, outsz - j, "%s", theme_bl[t]);
    for (int i = 0; i < width && j < outsz - 4; i++) j += (size_t)snprintf(out + j, outsz - j, "%s", theme_h[t]);
    snprintf(out + j, outsz - j, "%s", theme_br[t]);
}

void box_line_middle(char *out, size_t outsz, int width, int theme, const char *content) {
    int t = (theme >= 0 && theme <= 3) ? theme : 0;
    char pad[256];
    utf8_pad(pad, sizeof(pad), content ? content : "", width);
    snprintf(out, outsz, "%s%s%s", theme_v[t], pad, theme_v[t]);
}

void box_print(const char *title, const char **lines, int nlines, int theme) {
    int w = box_max_width(title, lines, nlines);
    if (w < 2) w = 2;
    if (w > 72) w = 72;
    char line[256];
    box_line_top(line, sizeof(line), w, theme);
    printf("%s\n", line);
    box_line_middle(line, sizeof(line), w, theme, title ? title : "");
    printf("%s\n", line);
    for (int i = 0; i < nlines && lines; i++) {
        box_line_middle(line, sizeof(line), w, theme, lines[i]);
        printf("%s\n", line);
    }
    box_line_bottom(line, sizeof(line), w, theme);
    printf("%s\n", line);
}
