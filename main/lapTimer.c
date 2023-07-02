/*
 * lapTimer.c
 *
 *  Created on: 31 May 2023
 *      Author: mirek
 */

#include "lapTimer.h"


void state_machine(void) {
  switch (_state) {
    case INIT:
      wifi_initialize();
      initialize_gpio();
      break;
    case MASTER_INIT:
      start_ap();
      break;
    case MASTER:
    	get_ir_sensor();
      break;
    case SLAVE_INIT:
      break;
    case SLAVE:
      break;
    case ERROR:
      break;
  }
}

void setup_modebtn_interrupt(void){
	ESP_ERROR_CHECK(gpio_set_direction(MODE_BUTTON_GPIO_PIN, GPIO_MODE_INPUT));
	ESP_ERROR_CHECK(gpio_set_pull_mode(MODE_BUTTON_GPIO_PIN, GPIO_PULLUP_ONLY));
	ESP_ERROR_CHECK(gpio_set_intr_type(MODE_BUTTON_GPIO_PIN, GPIO_INTR_NEGEDGE));
	ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_EDGE));
	ESP_ERROR_CHECK(gpio_isr_handler_add(MODE_BUTTON_GPIO_PIN, change_mode, NULL));
}

void change_mode(void* args) {
	printf("switchooo");
}
