#ifndef MAIN_LAPTIMER_IR_SENSOR_H_
#define MAIN_LAPTIMER_IR_SENSOR_H_

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "lapTimer_splits.h"
#include "lapTimer_types.h"
#include "lwip/sys.h"

#define INTERCEPT_TIME_US 3000000
#define SENSOR_BLIND_TIME_US 100000

#define IR_SENSOR_GPIO_PIN GPIO_NUM_2

int64_t IR_sensor_update(IR_Sensor_t* sensor);
void IR_sensor_initialize(void);
int64_t getSyncedTime(void);
extern int64_t captureTime;

#endif /* MAIN_LAPTIMER_IR_SENSOR_H_ */
