#ifndef LAPTIMER_BUTTON_H_
#define LAPTIMER_BUTTON_H_

#define BUTTON_DEBOUNCE 10
#define BUTTON_LONGPRESS_THRESH 1000000
#define BUTTON_NEXTPRESS_THRESH 500000

#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef void (*callback)(void);

typedef enum ButtonState {
    Released,
    Pressed_Debounce,
    Pressed,
    SecondPress_Wait,
    SecondPress_Debounce,
    SecondPress,
    LongPress_Wait,
    LongPress,
    WaitForRelease,
    Depressed_Debounce,
} ButtonState_t;

typedef struct Button {
    ButtonState_t _state;
    gpio_num_t _buttonPin;
    callback _singlePressCallback;
    callback _doublePressCallback;
    callback _longPressCallback;
    uint64_t _pressTime;
} Button_t;

void Button_Update(Button_t* button);

#endif /* MAIN_BUTTON_H_ */
