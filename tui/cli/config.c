#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <sys/stat.h>
#endif

#define CONFIG_MAX_PATH 512
#define CONFIG_KEY_FPS "fps_cap"
#define CONFIG_KEY_THEME "box_theme"

static int s_fps_cap = CONFIG_FPS_CAP_DEFAULT;
static int s_box_theme = CONFIG_BOX_THEME_DEFAULT;
static char s_config_path[CONFIG_MAX_PATH];
static int s_path_set;

static void config_default_path(void) {
    if (s_path_set) return;
    s_path_set = 1;
#if defined(_WIN32) || defined(_WIN64)
    {
        const char *apd = getenv("APPDATA");
        if (apd && (strlen(apd) + 20) < CONFIG_MAX_PATH) {
            snprintf(s_config_path, sizeof(s_config_path), "%s\\NeoArc\\config", apd);
            return;
        }
    }
    snprintf(s_config_path, sizeof(s_config_path), "neoarc_config");
#else
    {
        const char *home = getenv("HOME");
        if (home && (strlen(home) + 25) < CONFIG_MAX_PATH) {
            snprintf(s_config_path, sizeof(s_config_path), "%s/.config/neoarc/config", home);
            return;
        }
    }
    snprintf(s_config_path, sizeof(s_config_path), ".neoarc_config");
#endif
}

const char *config_get_path(void) {
    config_default_path();
    return s_config_path;
}

static void ensure_config_dir(void) {
#if defined(_WIN32) || defined(_WIN64)
    char dir[CONFIG_MAX_PATH];
    size_t i;
    strncpy(dir, s_config_path, sizeof(dir) - 1);
    dir[sizeof(dir) - 1] = '\0';
    for (i = strlen(dir); i > 0 && dir[i - 1] != '\\' && dir[i - 1] != '/'; i--) {}
    dir[i] = '\0';
    if (i > 0) CreateDirectoryA(dir, NULL);
#else
    char dir[CONFIG_MAX_PATH];
    char *p;
    strncpy(dir, s_config_path, sizeof(dir) - 1);
    dir[sizeof(dir) - 1] = '\0';
    p = strrchr(dir, '/');
    if (!p) return;
    *p = '\0';
    for (p = dir + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(dir, 0755);
            *p = '/';
        }
    }
    mkdir(dir, 0755);
#endif
}

int config_load(void) {
    config_default_path();
    FILE *f = fopen(s_config_path, "r");
    if (!f) return -1;
    char line[128];
    while (fgets(line, sizeof(line), f)) {
        int val;
        if (sscanf(line, "fps_cap=%d", &val) == 1) s_fps_cap = val;
        else if (sscanf(line, "box_theme=%d", &val) == 1) s_box_theme = val;
    }
    fclose(f);
    return 0;
}

int config_save(void) {
    config_default_path();
    ensure_config_dir();
    FILE *f = fopen(s_config_path, "w");
    if (!f) return -1;
    fprintf(f, "fps_cap=%d\n", s_fps_cap);
    fprintf(f, "box_theme=%d\n", s_box_theme);
    fclose(f);
    return 0;
}

int config_get_fps_cap(void) {
    return s_fps_cap;
}

void config_set_fps_cap(int fps) {
    s_fps_cap = fps;
}

int config_get_box_theme(void) {
    return s_box_theme;
}

void config_set_box_theme(int theme) {
    if (theme >= 0 && theme <= 3) s_box_theme = theme;
}
