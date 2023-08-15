#include "lapTimer.h"

Button_t button = {._buttonPin = MODE_BUTTON_GPIO_PIN, ._longPressCallback = &modeChange};
IR_Sensor_t IR_sensor = {._sensorPin = IR_SENSOR_GPIO_PIN};
SplitGroup_t splits = {._position = 5, ._split = {4, 5, 6, 7, 8, 9}};

void state_machine(void) {
    switch (_state) {
        case Init:
            _statusBits = 0;
            wifi_initialize();
            initialize_ir_sensor();
            modeButton_setup();
            break;
        case Master_Init:
            esp_wifi_stop();
            start_ap();
            setStatusBit(System_ready);
            break;
        case Master:
            IR_sensor_update(&IR_sensor);
            Button_Update(&button);
            break;
        case Slave_Init:
            kill_http();
            esp_wifi_stop();
            start_sta();
            break;
        case Slave:
            Button_Update(&button);
            break;
        case Error:
            break;
    }
}

void modeButton_setup(void) {
    ESP_ERROR_CHECK(gpio_set_direction(MODE_BUTTON_GPIO_PIN, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(MODE_BUTTON_GPIO_PIN, GPIO_PULLUP_ONLY));
}

void modeChange(void) {
    if (_state == Master)
        _state = Slave_Init;
    else
        _state = Master_Init;
    printf("Switcheroo\n");
}