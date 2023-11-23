#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>

#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <lttng/tracef.h>

#include "lab_1/thread_help.h"
#include "timespec_add_usec.h"

int LED_R = 28;
int LED_G = 29;
int LED_B = 27;
int running = 1;
struct timespec ex5_ts;

int pin_invert(int pin_state) {
    if (pin_state == LOW) return HIGH;
    else return LOW;
}

void * ex_red(void * arg) {
    uint32_t r_period_ms = 1000;
    useconds_t r_period_us = r_period_ms * 1000;
    uint32_t r_stress_ms = (uint32_t) (0.2 * (float) r_period_ms);

    struct timespec delay;
    delay.tv_sec = ex5_ts.tv_sec;
    delay.tv_nsec = ex5_ts.tv_nsec;
    timespec_add_usec(&delay, r_period_us);

    int v = LOW;
    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", r_stress_ms);
        cpu_stress(r_stress_ms);
        tracef("RED LED = %d", v);
        digitalWrite(LED_R, v);
        tracef("sleep(RED, %u usec)", r_period_us);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &delay, &ex5_ts);
    }
    return NULL;
}

int * ex_green(void * arg) {
    uint32_t g_period_ms = 1000;
    useconds_t g_period_us = g_period_ms * 1000;
    uint32_t g_stress_ms = (uint32_t) (0.2 * (float) g_period_ms);

    struct timespec delay;
    delay.tv_sec = ex5_ts.tv_sec;
    delay.tv_nsec = ex5_ts.tv_nsec;
    timespec_add_usec(&delay, g_period_us);


    int v = LOW;
    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", g_stress_ms);
        cpu_stress(g_stress_ms);
        tracef("GREEN LED = %d", v);
        digitalWrite(LED_G, v);
        tracef("sleep(GREEN, %u usec)", g_period_us);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &delay, &ex5_ts);
    }
    return NULL;
}

void * ex_blue(void * arg) {
    uint32_t b_period_ms = 1000;
    useconds_t b_period_us = b_period_ms * 1000;
    uint32_t b_stress_ms = (uint32_t) (0.2 * (float) b_period_ms);

    struct timespec delay;
    delay.tv_sec = ex5_ts.tv_sec;
    delay.tv_nsec = ex5_ts.tv_nsec;

    timespec_add_usec(&delay, b_period_us);

    int v = LOW;
    while(running) {

        v = pin_invert(v);
        tracef("Stressing for: %u ms", b_stress_ms);
        cpu_stress(b_stress_ms);
        tracef("BLUE LED = %d", v);
        digitalWrite(LED_B, v);
        tracef("sleep(BLUE, %u usec)", b_period_us);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &delay, &ex5_ts);
    }
    return NULL;
}

int main()
{
    wiringPiSetup();

    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_create(&t1, NULL, ex_red, NULL);
    pthread_create(&t2, NULL, ex_green, NULL);
    pthread_create(&t3, NULL, ex_blue, NULL);

    struct timespec delay;
    delay.tv_sec = ex5_ts.tv_sec;
    delay.tv_nsec = ex5_ts.tv_nsec;
    timespec_add_usec(&delay, 20000000);

    clock_nanosleep(CLOCK_MONOTONIC, 0, &delay, &ex5_ts);
    running = 0;

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("\nAll threads are done!\n");
    return EXIT_SUCCESS;
}