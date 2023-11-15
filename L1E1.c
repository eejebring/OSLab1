#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>

int LED_R = 28;
int LED_G = 29;
int LED_B = 27;

int pin_invert(int pin_state) {
    if (pin_state == LOW) return HIGH;
    else return LOW;
}

void * ex1_red(void * arg) {
    useconds_t green_sleep_us = 1000 * 1000; // 0.5s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_R, v);
        usleep(green_sleep_us);
    }
    return NULL;
}

void * ex1_green(void * arg) {
    useconds_t green_sleep_us = 500 * 1000; // 0.5s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_G, v);
        usleep(green_sleep_us);
    }
    return NULL;
}

void * ex1_blue(void * arg) {
    useconds_t green_sleep_us = 200 * 1000; // 0.5s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_B, v);
        usleep(green_sleep_us);
    }
    return NULL;
}