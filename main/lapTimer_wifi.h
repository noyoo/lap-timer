#ifndef LAPTIMER_WIFI_H_
#define LAPTIMER_WIFI_H_

#include <string.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "freertos/freeRTOS.h"
#include "freertos/task.h"
#include "lapTimer_http.h"
#include "lapTimer_types.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_timer.h"
#include "nvs_flash.h"

#include "lapTimer_ir_sensor.h"

extern State_t _state;
extern wifi_config_t wifi_config_ap, wifi_config_sta;
static EventGroupHandle_t wifi_event_group;
extern esp_event_handler_instance_t instance_any_id, instance_got_ip;
extern uint8_t retry;
extern uint32_t deviceIP;

void wifi_initialize(void);
void wifi_start_ap(void);
void wifi_start_sta(void);
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

#endif /* MAIN_LAPTIMER_WIFI_H_ */
