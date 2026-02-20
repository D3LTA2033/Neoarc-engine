#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *s_log;

void logger_init(const char *path) {
    if (s_log) fclose(s_log);
    s_log = path ? fopen(path, "a") : NULL;
}

void logger_shutdown(void) {
    if (s_log) { fclose(s_log); s_log = NULL; }
}

void logger_write(const char *msg) {
    if (s_log && msg) fprintf(s_log, "%s\n", msg);
}

void logger_error(const char *msg) {
    if (msg) fprintf(stderr, "[error] %s\n", msg);
    logger_write(msg);
}
