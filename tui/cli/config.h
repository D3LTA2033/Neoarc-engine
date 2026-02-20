#pragma once

#define CONFIG_FPS_CAP_DEFAULT 60
#define CONFIG_FPS_UNCAPPED 0
#define CONFIG_BOX_THEME_DEFAULT 0

int config_load(void);
int config_save(void);
int config_get_fps_cap(void);
void config_set_fps_cap(int fps);
int config_get_box_theme(void);
void config_set_box_theme(int theme);
const char *config_get_path(void);
