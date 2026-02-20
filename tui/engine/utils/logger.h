#pragma once

void logger_init(const char *path);
void logger_shutdown(void);
void logger_write(const char *msg);
void logger_error(const char *msg);
