#include "lapTimer.h"

Button_t button = {._buttonPin = MODE_BUTTON_GPIO_PIN, ._longPressCallback = &modeChange};
IR_Sensor_t IR_sensor = {._sensorPin = IR_SENSOR_GPIO_PIN, ._isActive = false};
SplitGroup_t splits = {._position = -1, ._split = {}};
bool measurmentStarted = 0;
int64_t ret;
int64_t lastSlavePollTime = 0;

void state_machine(void) {
    switch (_state) {
        case Init:
            wifi_initialize();
            IR_sensor_initialize();
            modeButton_setup();
            break;

        case Master_Init:
            kill_http_client();
            esp_wifi_stop();
            wifi_start_ap();
            splits._position = -1;
            measurmentStarted = 0;
            break;

        case Master:
            ret = IR_sensor_update(&IR_sensor);
            if (ret != 0 && measurmentStarted) addSplit(&splits, ret);
            Button_Update(&button);
            break;

        case Slave_Init:
            kill_http_server();
            esp_wifi_stop();
            wifi_start_sta();
            break;

        case Slave:
            if (esp_timer_get_time() > lastSlavePollTime + SLAVE_POLL_DELAY_US) {
                lastSlavePollTime = esp_timer_get_time();
                http_client_poll();
            }
            ret = IR_sensor_update(&IR_sensor);
            if (ret != 0) http_client_send_data(ret);
            Button_Update(&button);
            break;

        case Error:
            Button_Update(&button);
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