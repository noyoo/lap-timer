/*
 * lapTimer_i2c.h
 *
 *  Created on: 1 Jun 2023
 *      Author: mirek
 */

#ifndef MAIN_LAPTIMER_I2C_H_
#define MAIN_LAPTIMER_I2C_H_

#include "driver/i2c.h"
#include <stdio.h>
#include "lapTimer_types.h"
#include "esp_log.h"
#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"

#define VL53L0X_addr 0x52

extern uint8_t rxbuffer[3], txbuffer[1];
extern VL53L0X_Dev_t sensor;

void i2c_initialize(void);
void i2c_read(void);


#endif /* MAIN_LAPTIMER_I2C_H_ */
