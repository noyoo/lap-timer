#ifndef MAIN_LAPTIMER_H_
#define MAIN_LAPTIMER_H_

#include "button.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/freeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "lapTimer_http.h"
#include "lapTimer_ir_sensor.h"
#include "lapTimer_types.h"
#include "lapTimer_wifi.h"

#define MODE_BUTTON_GPIO_PIN GPIO_NUM_3

extern State_t _state;
extern QueueHandle_t interruptQueue;
extern Button_t button;
extern IR_Sensor_t IR_sensor;
extern SplitGroup_t splits;

void state_machine(void);
void modeButton_setup(void);
void modeChange(void);

#endif /* MAIN_LAPTIMER_H_ */
