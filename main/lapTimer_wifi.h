/*
 * lapTimer_WiFi.h
 *
 *  Created on: 31 May 2023
 *      Author: mirek
 */

#ifndef MAIN_LAPTIMER_WIFI_H_
#define MAIN_LAPTIMER_WIFI_H_

#include <string.h>
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_event.h"
#include "freertos/freeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "lapTimer_types.h"
#include "lapTimer_http.h"

extern State_t _state;
static const char* WIFI_TAG = "WIFI MODULE";
extern wifi_config_t wifi_config_ap, wifi_config_sta;
static EventGroupHandle_t wifi_event_group;
extern esp_event_handler_instance_t instance_any_id, instance_got_ip;
extern uint8_t retry;

void wifi_initialize(void);
void start_ap(void);
void start_sta(void);
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

#endif /* MAIN_LAPTIMER_WIFI_H_ */
