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
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "lapTimer_types.h"
#include "lapTimer_wifi.h"
#include "lapTimer_http.h"
#include "lapTimer_ir_sensor.h"
#include "button.h"

#define MODE_BUTTON_GPIO_PIN GPIO_NUM_3

extern State _state;
extern QueueHandle_t interruptQueue;
extern Button_t button;

void state_machine(void);
void setup_modebtn_interrupt(void);
static void IRAM_ATTR button_interrupt_handler(void*);
void mode_button_task(void);
void guzior(void);
#endif /* MAIN_LAPTIMER_H_ */
