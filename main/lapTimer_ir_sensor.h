/*
 * lapTimer_ir_sensor.h
 *
 *  Created on: 1 Jul 2023
 *      Author: mirek
 */

#ifndef MAIN_LAPTIMER_IR_SENSOR_H_
#define MAIN_LAPTIMER_IR_SENSOR_H_


#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_timer.h"

#define IR_SENSOR_GPIO_PIN GPIO_NUM_2

typedef enum {
	LOW,
	HIGH,
}IR_State_t;

static const char* IR_TAG = "IR SENSOR";
extern IR_State_t _irState;
extern esp_timer_handle_t debounceTimer;

void initialize_ir_sensor(void);
void get_ir_sensor(void);
void debounce_end(void);

#endif /* MAIN_LAPTIMER_IR_SENSOR_H_ */
