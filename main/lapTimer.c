/*
 * lapTimer.c
 *
 *  Created on: 31 May 2023
 *      Author: mirek
 */

#include "lapTimer.h"

QueueHandle_t interruptQueue;
Button_t button = {._buttonPin = MODE_BUTTON_GPIO_PIN, ._singlePressCallback = &guzior};

void state_machine(void) {
    switch (_state) {
        case INIT:
            wifi_initialize();
            initialize_ir_sensor();
            modeButton_setup();
            break;
        case MASTER_INIT:
            start_ap();
            break;
        case MASTER:
            get_ir_sensor();
            Button_Update(&button);
            break;
        case SLAVE_INIT:
            break;
        case SLAVE:
            break;
        case ERROR:
            break;
    }
}

void modeButton_setup(void) {
    ESP_ERROR_CHECK(gpio_set_direction(MODE_BUTTON_GPIO_PIN, GPIO_MODE_INPUT));
    ESP_ERROR_CHECK(gpio_set_pull_mode(MODE_BUTTON_GPIO_PIN, GPIO_PULLUP_ONLY));
    //	ESP_ERROR_CHECK(gpio_set_intr_type(MODE_BUTTON_GPIO_PIN, GPIO_INTR_NEGEDGE));
    //	interruptQueue = xQueueCreate(10, sizeof(int));
    //	xTaskCreate(mode_button_task,"mode_button_task", 2048, NULL, 1, NULL);
    //	ESP_ERROR_CHECK(gpio_install_isr_service(0));
    //	ESP_ERROR_CHECK(gpio_isr_handler_add(MODE_BUTTON_GPIO_PIN, button_interrupt_handler, (void*)MODE_BUTTON_GPIO_PIN));
    //	printf("handler\n");
}

static void IRAM_ATTR button_interrupt_handler(void* args) {
    int pinNum = (int)args;
    xQueueSendFromISR(interruptQueue, &pinNum, NULL);
}

void mode_button_task(void) {
    int pinNum;
    for (;;) {
        if (xQueueReceive(interruptQueue, &pinNum, portMAX_DELAY)) {
            printf("switchooo\n");
        }
    }
}

void guzior() {
    ESP_LOGI("PINKO", "dziala");
}
