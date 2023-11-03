#include <time.h>

void timespec_add_usec(struct timespec *ts, long us) {
    long partialSeconds = us % 1000000;
    ts->tv_nsec = ts->tv_nsec + partialSeconds * 1000;
    while (1000000000 <= ts->tv_nsec ) {
        ts->tv_sec++;
        ts->tv_nsec -= 1000000000;
    }
    ts->tv_sec = ts->tv_sec +  (us - partialSeconds) / 1000000;
}