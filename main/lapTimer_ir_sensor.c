#include "lapTimer_ir_sensor.h"

int64_t captureTime = 0;

void IR_sensor_initialize(void) {
    ESP_ERROR_CHECK(gpio_set_direction(IR_SENSOR_GPIO_PIN, GPIO_MODE_INPUT));
}

int64_t IR_sensor_update(IR_Sensor_t* sensor) {
    switch (sensor->_state) {
        case Intercept:
            if (gpio_get_level(sensor->_sensorPin)) {
                captureTime = 0;
                break;
            }

            if (captureTime == 0)
                captureTime = esp_timer_get_time();

            if (esp_timer_get_time() > captureTime + INTERCEPT_TIME_US) {
                sensor->_state = Armed;
            }
            break;

        case Armed:
            if (sensor->_isActive) sensor->_state = Active;
            break;

        case Active:
            if (gpio_get_level(sensor->_sensorPin)) {
                sensor->_state = WaitForClear;
                captureTime = esp_timer_get_time();
                return getSyncedTime();
            }

            if (!sensor->_isActive) sensor->_state = Armed;
            break;

        case WaitForClear:
            if ((esp_timer_get_time() > captureTime + SENSOR_BLIND_TIME_US) &&
                (!gpio_get_level(sensor->_sensorPin))) sensor->_state = Active;
            break;
    }
    return 0;
}

int64_t getSyncedTime(void){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000L + (int64_t)tv.tv_usec;
}