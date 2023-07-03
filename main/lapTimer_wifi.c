/*
 * lapTimer_WiFi.c
 *
 *  Created on: 31 May 2023
 *      Author: mirek
 */

#include "lapTimer_wifi.h"

  uint8_t retry = 0;

  wifi_config_t wifi_config_ap = { .ap = { .ssid = CONFIG_MODULE_WIFI_SSID,
		.ssid_len = strlen(CONFIG_MODULE_WIFI_SSID), .channel = 4, .password =
				CONFIG_MODULE_WIFI_PASSWORD, .max_connection = 4, .authmode =
				WIFI_AUTH_WPA_WPA2_PSK, .pmf_cfg = { .required = false, }, }, };

  wifi_config_t wifi_config_sta = { .sta = { .ssid = CONFIG_MODULE_WIFI_SSID,
		.password = CONFIG_MODULE_WIFI_PASSWORD, } };

void wifi_initialize(void) {
	printf("%s\n",CONFIG_MODULE_WIFI_PASSWORD);
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ESP_ERR_NVS_NEW_VERSION_FOUND){
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  esp_netif_create_default_wifi_ap();
  esp_netif_create_default_wifi_sta();

  const wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

  esp_event_handler_instance_t instance_any_id, instance_got_ip;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                      ESP_EVENT_ANY_ID,
                                                      &wifi_event_handler,
                                                      NULL,
                                                      &instance_any_id));

  ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                      IP_EVENT_STA_GOT_IP,
                                                      &wifi_event_handler,
                                                      NULL,
                                                      &instance_got_ip));

  _state = MASTER_INIT;
}

void start_ap(void){
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config_ap));
  ESP_ERROR_CHECK(esp_wifi_start());
  _state = MASTER;
}

void start_sta(void){
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config_sta));
  ESP_ERROR_CHECK(esp_wifi_start());

  EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
          BIT0 | BIT1,
          pdFALSE,
          pdFALSE,
          portMAX_DELAY);

  if (bits & BIT0) { // CONNECTED
  } else if (bits & BIT1) { // FAILED
  } else {
      ESP_LOGE(WIFI_TAG, "UNEXPECTED EVENT");
  }
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
  if (event_id == WIFI_EVENT_AP_STACONNECTED) {
          wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
      }
  else if(event_id == WIFI_EVENT_AP_START){
	  http_server_initialize();
  }
  else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
          wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
      }
  else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }
  else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (retry < 10) {
            esp_wifi_connect();
            retry++;
            ESP_LOGI(WIFI_TAG, "retry to connect to the AP");
        }
        else {
            xEventGroupSetBits(wifi_event_group, BIT1);
        }
        ESP_LOGI(WIFI_TAG,"connect to the AP fail");
    }
  else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        retry = 0;
        xEventGroupSetBits(wifi_event_group, BIT0);
        _state = SLAVE;
  }
}
