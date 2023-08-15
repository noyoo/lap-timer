#ifndef MAIN_LAPTIMER_IR_SENSOR_H_
#define MAIN_LAPTIMER_IR_SENSOR_H_

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "lapTimer_types.h"
#include "lapTimer_splits.h"
#include "lapTimer_status.h"

#define INTERCEPT_TIME 3000000

#define IR_SENSOR_GPIO_PIN GPIO_NUM_2

typedef enum {
    Intercept,
    Armed,
    Active,
    WaitForClear,
} IR_State_t;

typedef struct {
    IR_State_t _state;
    uint8_t _sensorPin;
} IR_Sensor_t;

int64_t IR_sensor_update(IR_Sensor_t* sensor);
void initialize_ir_sensor(void);
extern int64_t intercept_startTime;

#endif /* MAIN_LAPTIMER_IR_SENSOR_H_ */
