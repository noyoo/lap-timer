#include "lapTimer_ir_sensor.h"

void initialize_ir_sensor(void) {
    ESP_ERROR_CHECK(gpio_set_direction(IR_SENSOR_GPIO_PIN, GPIO_MODE_INPUT));
}

int64_t intercept_startTime = 0;

int64_t IR_sensor_update(IR_Sensor_t* sensor) {
    switch (sensor->_state) {
        case Intercept:
            sensor->_state = Armed;
            setStatusBit(Gate_calibrated);
            if (!gpio_get_level(sensor->_sensorPin)) {
                setStatusBit(Gate_calibrating);
                if (intercept_startTime == 0)
                    intercept_startTime = esp_timer_get_time();
                else if (esp_timer_get_time() > intercept_startTime + INTERCEPT_TIME) {
                    resetStatusBit(Gate_calibrating);
                    setStatusBit(Gate_calibrated);
                    resetStatusBit(Gate_active);
                    sensor->_state = Armed;
                }
            } else {
                intercept_startTime = 0;
            }
            break;
        case Armed:
            if (getStatusBit(Gate_active)) {
                sensor->_state = Active;
            }
            sensor->_state = Active;
            break;
        case Active:
            if (gpio_get_level(sensor->_sensorPin)) {
                printf("ping\n");
                sensor->_state = WaitForClear;
                intercept_startTime = esp_timer_get_time();
                return esp_timer_get_time();
            }
            // if (!getStatusBit(Gate_active)) {
            //     sensor->_state = Armed;
            // }

            // if (!getStatusBit(Gate_calibrated)) {
            //     sensor->_state = Intercept;
            // }
            break;
        case WaitForClear:
            if (esp_timer_get_time() > intercept_startTime + 10000) {
                if (!gpio_get_level(sensor->_sensorPin)) sensor->_state = Active;
            }
            break;
    }
    return 0;
}
