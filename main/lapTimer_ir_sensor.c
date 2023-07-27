/*
 * lapTimer_ir_sensor.c
 *
 *  Created on: 1 Jul 2023
 *      Author: mirek
 */

#include "lapTimer_ir_sensor.h"

IR_State_t _irState = HIGH;
esp_timer_handle_t debounceTimer;
void initialize_ir_sensor(void) {
    ESP_ERROR_CHECK(gpio_set_direction(IR_SENSOR_GPIO_PIN, GPIO_MODE_INPUT));
    esp_timer_create_args_t debounceTimerArgs = {
        .callback = &debounce_end,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "debounce_timer",
        .skip_unhandled_events = true,
    };
    ESP_ERROR_CHECK(esp_timer_create(&debounceTimerArgs, &debounceTimer));
}

void get_ir_sensor(void) {
    // switch (_irState) {
    // case HIGH:
    if (!gpio_get_level(IR_SENSOR_GPIO_PIN)) {
        int64_t snap_time = esp_timer_get_time();
        uint8_t hours, minutes, seconds, millis;
        millis = (snap_time / 1000) % 1000;
        seconds = (snap_time / 1000000) % 60;
        minutes = (snap_time / 60000000) % 60;
        hours = (snap_time / 3600000000);
        ESP_LOGI("IR_SENSOR", "IR sensor triggered! Time is %02d:%02d:%02d:%03d", hours, minutes, seconds, millis);
        // ESP_ERROR_CHECK(esp_timer_start_once(debounceTimer, 500000));
        // _irState = LOW;
    }
    //     break;
    // case LOW:

    //     break;
    // }

    //	if(!gpio_get_level(IR_SENSOR_GPIO_PIN)){
    //
    //	}
}

void debounce_end(void*) {
    ESP_LOGI("IR_SENSOR", "BOINK");
    esp_timer_stop(debounceTimer);
    _irState = HIGH;
}
