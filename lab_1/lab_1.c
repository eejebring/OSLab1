#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <lttng/tracef.h>

#include "lab_1.h"
#include "thread_help.h"

/*************************************************************
 *                                                           *
 * RUNTIME_MILLSEC can be one of three things:               *
 *                                                           *
 *      -> If it's greater than 0 then it's how many         *
 *          milliseconds the program will run for. When the  *
 *          time is over kill_led_threads() will be called.  *
 *                                                           *
 *      -> If it's 0 then the program will run until         *
 *          the user presses RETURN in the terminal, at      *
 *          which time kill_led_threads() will be called.    *
 *                                                           *
 *      -> If it's less than 0 then kill_led_threads() won't *
 *          be called. Instead the threads should terminate  *
 *          themselves.                                      *
 *                                                           *
 *                                                           *
 * main.c:halting_function() for more details.               *
 *                                                           *
 *************************************************************/
#define RUNTIME_MILLSEC     20000
// run `chrt -m` to learn allowed values (SCHED_FIFO)

#define REALTIME_THREADS    0
#define RT_PRIO_R           -1
#define RT_PRIO_G           -1
#define RT_PRIO_B           -1

/*
#define REALTIME_THREADS 1
#define RT_PRIO_R 3
#define RT_PRIO_G 2
#define RT_PRIO_B 1
*/
#define EXERCISE_TO_RUN     4

int running;  // leave this alone


/*************************************************************
 *************************************************************
 **               PREPARATORY EXERCISE                      **
 *************************************************************
 *************************************************************/
void timespec_add_usec(struct timespec *ts, long microseconds);

void timespec_add_usec(struct timespec *ts, long us)
{
    long partialSeconds = us % 1000000;
    ts->tv_nsec = ts->tv_nsec + partialSeconds * 1000;
    while (1000000000 <= ts->tv_nsec ) {
        ts->tv_sec++;
        ts->tv_nsec -= 1000000000;
    }
    ts->tv_sec = ts->tv_sec +  (us - partialSeconds) / 1000000;
}

int pin_invert(int pin_state) {
    if (pin_state == LOW) return HIGH;
    else return LOW;
}

/*************************************************************
 *************************************************************
 **               EXERCISE 1 - Blinking                     **
 *************************************************************
 *************************************************************/

void   ex1_init()
{
}

void * ex1_red(void * arg)
{
    useconds_t green_sleep_us = 1000 * 1000; // 0.5s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_R, v);
        usleep(green_sleep_us);
    }
    return NULL;
}

void * ex1_green(void * arg)
{
    useconds_t green_sleep_us = 500 * 1000; // 0.5s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_G, v);
        usleep(green_sleep_us);
    }
    return NULL;
}

void * ex1_blue(void * arg)
{
    useconds_t green_sleep_us = 200 * 1000; // 0.5s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_B, v);
        usleep(green_sleep_us);
    }
    return NULL;
}



/*************************************************************
 *************************************************************
 **               EXERCISE 2 - Adding tracef()              **
 *************************************************************
 *************************************************************/

void   ex2_init()
{
}

void * ex2_red(void * arg)
{
    useconds_t green_sleep_us = 1000 * 1000; // 0.5s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_R, v);
        usleep(green_sleep_us);
    }
    return NULL;
}

void * ex2_green(void * arg)
{
    useconds_t green_sleep_us = 500 * 1000; // 0.5s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        tracef("GREEN LED = %d", v);
        digitalWrite(LED_G, v);
        tracef("sleep(GREEN, %u usec)", green_sleep_us);
        usleep(green_sleep_us);
    }
    return NULL;
}

void * ex2_blue(void * arg)
{
    useconds_t green_sleep_us = 200 * 1000; // 0.5s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_B, v);
        usleep(green_sleep_us);
    }
    return NULL;
}



/*************************************************************
 *************************************************************
 **               EXERCISE 3 - Adding CPU Load              **
 *************************************************************
 *************************************************************/

void   ex3_init()
{
}

void * ex3_red(void * arg)
{
    uint32_t r_period_ms = 1000;
    useconds_t r_period_us = r_period_ms * 1000;
    uint32_t r_stress_ms = (uint32_t) (0.2 * (float) r_period_ms);

    int v = LOW;
    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", r_stress_ms);
        cpu_stress(r_stress_ms);
        tracef("RED LED = %d", v);
        digitalWrite(LED_R, v);
        tracef("sleep(RED, %u usec)", r_period_us);
        usleep(r_period_us);
    }
    return NULL;
}

void * ex3_green(void * arg)
{
    uint32_t g_period_ms = 500;
    useconds_t g_period_us = g_period_ms * 1000;
    uint32_t g_stress_ms = (uint32_t) (0.2 * (float) g_period_ms);

    int v = LOW;
    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", g_stress_ms);
        cpu_stress(g_stress_ms);
        tracef("GREEN LED = %d", v);
        digitalWrite(LED_G, v);
        tracef("sleep(GREEN, %u usec)", g_period_us);
        usleep(g_period_us);
    }
    return NULL;
}

void * ex3_blue(void * arg)
{
    uint32_t b_period_ms = 200;
    useconds_t b_period_us = b_period_ms * 1000;
    uint32_t b_stress_ms = (uint32_t) (0.2 * (float) b_period_ms);

    int v = LOW;
    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", b_stress_ms);
        cpu_stress(b_stress_ms);
        tracef("BLUE LED = %d", v);
        digitalWrite(LED_B, v);
        tracef("sleep(BLUE, %u usec)", b_period_us);
        usleep(b_period_us);
    }
    return NULL;
}




/*************************************************************
 *************************************************************
 **               EXERCISE 4 - Many Ways of Sleeping        **
 *************************************************************
 *************************************************************/

void   ex4_init()
{
}

void * ex4_red(void * arg) {
    uint32_t r_period_ms = 1000;
    useconds_t r_period_us = r_period_ms * 1000;
    uint32_t r_stress_ms = (uint32_t) (0.2 * (float) r_period_ms);

    int v = LOW;
    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", r_stress_ms);
        cpu_stress(r_stress_ms);
        tracef("RED LED = %d", v);
        digitalWrite(LED_R, v);
        tracef("sleep(RED, %u usec)", r_period_us);
        usleep(r_period_us);
    }
    return NULL;
}

void * ex4_green(void * arg)
{
    uint32_t g_period_ms = 1000;
    useconds_t g_period_us = g_period_ms * 1000;
    uint32_t g_stress_ms = (uint32_t) (0.2 * (float) g_period_ms);

    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 0;
    timespec_add_usec(&delay, g_period_us);


    int v = LOW;
    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", g_stress_ms);
        cpu_stress(g_stress_ms);
        tracef("GREEN LED = %d", v);
        digitalWrite(LED_G, v);
        tracef("sleep(GREEN, %u usec)", g_period_us);
        nanosleep(&delay, NULL);
    }
    return NULL;
}

void * ex4_blue(void * arg)
{
    uint32_t b_period_ms = 1000;
    useconds_t b_period_us = b_period_ms * 1000;
    uint32_t b_stress_ms = (uint32_t) (0.2 * (float) b_period_ms);

    struct timespec delay;
    clock_gettime(CLOCK_REALTIME, &delay);

    int v = LOW;
    while(running) {
        timespec_add_usec(&delay, b_period_us);

        v = pin_invert(v);
        tracef("Stressing for: %u ms", b_stress_ms);
        cpu_stress(b_stress_ms);
        tracef("BLUE LED = %d", v);
        digitalWrite(LED_B, v);
        tracef("sleep(BLUE, %u usec)", b_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &delay, NULL);
    }
    return NULL;
}



/*************************************************************
 *************************************************************
 **               EXERCISE 5 - Properly Periodical          **
 *************************************************************
 *************************************************************/

struct timespec ex5_ts;

void   ex5_init()
{
}

void * ex5_red(void * arg)
{
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
        timespec_add_usec(&delay, r_period_us);
        tracef("RED LED = %d", v);
        digitalWrite(LED_R, v);
        tracef("sleep(RED, %u usec)", r_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &delay, &ex5_ts);
    }
    return NULL;
}

void * ex5_green(void * arg)
{
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
        timespec_add_usec(&delay, g_period_us);
        tracef("GREEN LED = %d", v);
        digitalWrite(LED_G, v);
        tracef("sleep(GREEN, %u usec)", g_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &delay, &ex5_ts);
    }
    return NULL;
}

void * ex5_blue(void * arg)
{
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
        timespec_add_usec(&delay, b_period_us);
        tracef("BLUE LED = %d", v);
        digitalWrite(LED_B, v);
        tracef("sleep(BLUE, %u usec)", b_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &delay, &ex5_ts);
    }
    return NULL;
}




/*************************************************************
 *************************************************************
 **               EXERCISE 6 - Real-Time Scheduling         **
 *************************************************************
 *************************************************************/

void   ex6_init()
{
}

void * ex6_red(void * arg)
{
    uint32_t r_period_ms = 100;
    useconds_t r_period_us = r_period_ms * 1000;
    uint32_t r_stress_ms = (uint32_t) (0.4 * (float) r_period_ms);

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
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &delay, &ex5_ts);
    }
    return NULL;
}

void * ex6_green(void * arg)
{
    uint32_t g_period_ms = 50;
    useconds_t g_period_us = g_period_ms * 1000;
    uint32_t g_stress_ms = (uint32_t) (0.7 * (float) g_period_ms);

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
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &delay, &ex5_ts);
    }
    return NULL;
}

void * ex6_blue(void * arg)
{
    uint32_t b_period_ms = 20;
    useconds_t b_period_us = b_period_ms * 1000;
    uint32_t b_stress_ms = (uint32_t) (0.9 * (float) b_period_ms);

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
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &delay, &ex5_ts);
    }
    return NULL;
}


/*************************************************************
 *************************************************************
 **               EXERCISE 7 - CPU Overloading              **
 *************************************************************
 *************************************************************/

void   ex7_init()
{
}

void * ex7_red(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex7_green(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex7_blue(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}



/*************************************************************
 *************************************************************
 **         EXERCISE 8 - [Bonus] Resource Protection        **
 *************************************************************
 *************************************************************/

void   ex8_init()
{
}

void * ex8_red(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex8_green(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex8_blue(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}


/*************************************************************
 *************************************************************
 **         EXERCISE 9 - [Bonus] Control Flow               **
 *************************************************************
 *************************************************************/

void   ex9_init()
{
}

void * ex9_red(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex9_green(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex9_blue(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}



/*************************************************************
 *************************************************************
 **                                                         **
 **         EXPORTED FUNCTIONS BELOW!                       **
 **                                                         **
 *************************************************************
 *************************************************************/


/****** DON'T TOUCH THESE FUNCTIONS ******/
void  kill_led_threads()    { running = 0; }
int   want_realtime()       { return REALTIME_THREADS; }
int   milliseconds_to_run() { return RUNTIME_MILLSEC; }

void    get_rt_prios(int * p_r, int * p_g, int * p_b)
{
    if (p_r == NULL || p_g == NULL || p_b == NULL)
    {
        printf("get_rt_prios(): One or more arguments was NULL.\n");
    }
    *p_r = RT_PRIO_R;
    *p_g = RT_PRIO_G;
    *p_b = RT_PRIO_B;
}
/****** DON'T TOUCH THESE FUNCTIONS ******/


void user_init()
{
    running = 1;
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);

    switch (EXERCISE_TO_RUN)
    {
    case 1:     ex1_init();         break;
    case 2:     ex2_init();         break;
    case 3:     ex3_init();         break;
    case 4:     ex4_init();         break;
    case 5:     ex5_init();         break;
    case 6:     ex6_init();         break;
    case 7:     ex7_init();         break;
    case 8:     ex8_init();         break;
    case 9:     ex9_init();         break;
    default:    fprintf(stderr,
                        "ERROR: Invalid exercise number: %d\n",
                        EXERCISE_TO_RUN);
                exit(EXIT_FAILURE);
                break;
    }
}

void * run_red(void *arg)
{
    int target_core = highest_core_number();
    stick_this_thread_to_core(target_core);
    switch (EXERCISE_TO_RUN)
    {
    case 1:     ex1_red(arg);       break;
    case 2:     ex2_red(arg);       break;
    case 3:     ex3_red(arg);       break;
    case 4:     ex4_red(arg);       break;
    case 5:     ex5_red(arg);       break;
    case 6:     ex6_red(arg);       break;
    case 7:     ex7_red(arg);       break;
    case 8:     ex8_red(arg);       break;
    case 9:     ex9_red(arg);       break;
    default:                        break;
    }
}

void * run_green(void *arg)
{
    int target_core = highest_core_number();
    stick_this_thread_to_core(target_core);
    switch (EXERCISE_TO_RUN)
    {
    case 1:     ex1_green(arg);     break;
    case 2:     ex2_green(arg);     break;
    case 3:     ex3_green(arg);     break;
    case 4:     ex4_green(arg);     break;
    case 5:     ex5_green(arg);     break;
    case 6:     ex6_green(arg);     break;
    case 7:     ex7_green(arg);     break;
    case 8:     ex8_green(arg);     break;
    case 9:     ex9_green(arg);     break;
    default:                        break;
    }
}

void * run_blue(void *arg)
{
    int target_core = highest_core_number();
    stick_this_thread_to_core(target_core);
    switch (EXERCISE_TO_RUN)
    {
    case 1:     ex1_blue(arg);      break;
    case 2:     ex2_blue(arg);      break;
    case 3:     ex3_blue(arg);      break;
    case 4:     ex4_blue(arg);      break;
    case 5:     ex5_blue(arg);      break;
    case 6:     ex6_blue(arg);      break;
    case 7:     ex7_blue(arg);      break;
    case 8:     ex8_blue(arg);      break;
    case 9:     ex9_blue(arg);      break;
    default:                        break;
    }
}

