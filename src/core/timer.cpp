#include "timer.h"
#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct Timer* timer = NULL;


void init_timer(struct Timer* timer) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency( &freq );
    timer->frequency = 1.0/(double)freq.QuadPart;
    reset_timer(timer);
}
void reset_timer(struct Timer* timer) {
    QueryPerformanceCounter((LARGE_INTEGER*)&timer->start_time);
    timer->prev_time = timer->start_time;
}
double get_delta_time(struct Timer* timer) {
    uint64_t current_time;
    double delta_time;
    QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    delta_time = (current_time - timer->prev_time) * timer->frequency;
    timer->prev_time = current_time;
    return delta_time;
}
double get_running_time(struct Timer* timer) {
    uint64_t current_time;
    double  running_time;
    QueryPerformanceCounter( (LARGE_INTEGER*)&current_time );
    running_time = (current_time - timer->start_time) * timer->frequency;
    return running_time;
}
#endif


#if defined (__linux__)



struct Timer* timer = NULL;

static struct timespec _time_difference(struct timespec a, struct timespec b)
{
    struct timespec temp;
    temp.tv_sec = a.tv_sec-b.tv_sec;
    temp.tv_nsec = a.tv_nsec-b.tv_nsec;
    return temp;
}

void init_timer(struct Timer* timer)
{
    //Timer* timer = (Timer*)calloc(1, sizeof(*timer));
    reset_timer(timer);
}

void destroy_timer(Timer* timer)
{
    free(timer);
}

void reset_timer(Timer* timer)
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    timer->prev_time = timer->start_time = time;
}

double get_delta_time(Timer* timer)
{
    struct timespec time;
    struct timespec diff;
    clock_gettime(CLOCK_MONOTONIC, &time);
    diff = _time_difference(time, timer->prev_time);
    timer->prev_time = time;
    return diff.tv_sec + diff.tv_nsec*1.0/1000000000;
}

double get_running_time(Timer* timer)
{
    struct timespec time;
    struct timespec diff;
    clock_gettime(CLOCK_MONOTONIC, &time);
    diff = _time_difference(time, timer->start_time);
    return diff.tv_sec + diff.tv_nsec*1.0/1000000000;
}

#endif
