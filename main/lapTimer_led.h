#ifndef LAPTIMER_LED_H
#define LAPTIMER_LED_H

#include <inttypes.h>
#include <stdbool.h>

#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>


typedef enum LED_State {
    Wait,
    BlinkNTimes,
    ConstantBlink,
    Constant,
} LED_State_t;

typedef struct LED {
    LED_State_t _state;
    gpio_num_t _pin;
    uint8_t _blinkTarget;
    bool _blinkTracker;
    uint8_t _blinkCounter;
    int64_t _captureTime;
    uint8_t _blinkFrequency;
} LED_t;

extern LED_t LED;

void updateLED(LED_t* LED);
void LED_blinkNtimes(LED_t* led, uint8_t N);
void LED_blinkConstanty(LED_t* led);
void LED_setBlinkFrequency(LED_t* led, uint8_t blinkFrequency);
void LED_turnOn(LED_t* led);
void LED_turnOff(LED_t* led);
void LED_setup(LED_t* led);

#endif