#pragma once

#define MENU_SEARCH   1
#define MENU_PLAY     2
#define MENU_LEARN    3
#define MENU_HELP     4
#define MENU_SETTINGS 5
#define MENU_CREDITS  6
#define MENU_QUIT     7

void menu_show(void);
int menu_run(void);
int menu_play_submenu(void);
void menu_search(int *out_game_index);
void menu_settings(void);
void menu_credits(void);
