#include "timer.h"
#include <time.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
static LARGE_INTEGER s_freq;
static LARGE_INTEGER s_start;
#else
static struct timespec s_start;
#endif

void timer_init(void) {
#if defined(_WIN32) || defined(_WIN64)
    QueryPerformanceFrequency(&s_freq);
    QueryPerformanceCounter(&s_start);
#else
    clock_gettime(CLOCK_MONOTONIC, &s_start);
#endif
}

double timer_elapsed(void) {
#if defined(_WIN32) || defined(_WIN64)
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (double)(now.QuadPart - s_start.QuadPart) / (double)s_freq.QuadPart;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (double)(now.tv_sec - s_start.tv_sec) + 1e-9 * (double)(now.tv_nsec - s_start.tv_nsec);
#endif
}

void timer_sleep_ms(int ms) {
#if defined(_WIN32) || defined(_WIN64)
    Sleep((DWORD)ms);
#else
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
#endif
}
