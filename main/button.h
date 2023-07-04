/*
 * button.h
 *
 *  Created on: 4 lip 2023
 *      Author: dvagh
 */

#ifndef MAIN_BUTTON_H_
#define MAIN_BUTTON_H_

#define BUTTON_DEBOUNCE 20

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef void(*callback)(void);

typedef enum ButtonState
{
	Depressed,
	Pressed_Debounce,
	Pressed,
	Depressed_Debounce
}ButtonState_t;

typedef struct Button
{
	ButtonState_t _state;
	ButtonState_t _lastState;
	unsigned int _buttonPin;
	callback _callback;

}Button_t;

int Button_Update(Button_t* button);

#endif /* MAIN_BUTTON_H_ */
