#ifndef __timer_h__
#define __timer_h__

#include <stdint.h>

#if defined (_WIN32)
struct Timer
{
    uint64_t    start_time;
    uint64_t    prev_time;
    double      frequency;
};
#endif

#if defined(__linux__)

#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>

typedef struct Timer
{
    struct timespec start_time;
    struct timespec prev_time;
} Timer;

#endif

extern struct Timer* timer;



struct Timer* create_timer(void);
//void destroy_timer(struct Timer* timer);
void init_timer(struct Timer* timer);

void reset_timer(struct Timer* timer);
/** @return The time since the last `get_delta_time` call, in seconds. */
double get_delta_time(struct Timer* timer);
/** @return The time since the last `reset_timer` call, in seconds. */
double get_running_time(struct Timer* timer);



#endif /* include guard */
