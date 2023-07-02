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
#define IR_SENSOR_GPIO_PIN GPIO_NUM_2

static const char* IR_TAG = "IR SENSOR";

void initialize_gpio(void);
void get_ir_sensor(void);

#endif /* MAIN_LAPTIMER_IR_SENSOR_H_ */
