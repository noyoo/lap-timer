/*
 * lapTimer_ir_sensor.c
 *
 *  Created on: 1 Jul 2023
 *      Author: mirek
 */


#include "lapTimer_ir_sensor.h"

void initialize_gpio(void) {
	ESP_ERROR_CHECK(gpio_set_direction(IR_SENSOR_GPIO_PIN, GPIO_MODE_INPUT));
}

void get_ir_sensor(void) {
	if(!gpio_get_level(IR_SENSOR_GPIO_PIN)){
		int64_t snap_time = esp_timer_get_time();
		ESP_LOGI(IR_TAG, "IR sensor triggered! Time is: %lld ms", snap_time/1000);
	}
}
