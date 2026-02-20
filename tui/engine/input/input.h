#pragma once

#define KEY_UP    0x0100
#define KEY_DOWN  0x0101
#define KEY_LEFT  0x0102
#define KEY_RIGHT 0x0103
#define KEY_QUIT  0x0104
#define KEY_ENTER 0x0105

void input_init(void);
void input_shutdown(void);
int input_poll(void);
