#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> // compile with -lpthread
#include "timespec_add_usec.h"

int running = 1;

void * fun_1(void * arg)
{
    struct timespec tStart, tEnd;

    tEnd.tv_nsec = 500000;
    tEnd.tv_sec = 0;
    tStart.tv_sec = 0;

    while (running)
    {
        printf("X");
        //usleep(500);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &tEnd, &tStart);
    }
    return NULL;
}

void * fun_2(void * arg)
{
    struct timespec tStart, tEnd;

    tEnd.tv_nsec = 500000;
    tEnd.tv_sec = 0;
    tStart.tv_sec = 0;

    while (running)
    {
        printf(" ");
        //usleep(500);
        clock_nanosleep(CLOCK_MONOTONIC, 0, &tEnd, &tStart);
    }
    return NULL;
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, fun_1, NULL);
    pthread_create(&t2, NULL, fun_2, NULL);

    usleep(500000); // 500 milliseconds
    running = 0;

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nBoth threads are done!\n");
    return EXIT_SUCCESS;
}