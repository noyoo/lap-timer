/*
 * lapTimer.h
 *
 *  Created on: 31 May 2023
 *      Author: mirek
 */

#ifndef MAIN_LAPTIMER_H_
#define MAIN_LAPTIMER_H_

#include "freertos/freeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "lapTimer_types.h"
#include "lapTimer_wifi.h"
#include "lapTimer_http.h"
#include "lapTimer_ir_sensor.h"

#define MODE_BUTTON_GPIO_PIN GPIO_NUM_3

extern State _state;

void state_machine(void);
void setup_modebtn_interrupt(void);
void change_mode(void*);
#endif /* MAIN_LAPTIMER_H_ */
