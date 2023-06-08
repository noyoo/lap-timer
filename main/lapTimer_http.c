/*
 * lapTimer_http.c
 *
 *  Created on: 8 cze 2023
 *      Author: mirek
 */

#include "lapTimer_http.h"

void http_server_initialize(void){
	httpd_handle_t http_server;
	httpd_config_t http_config = HTTPD_DEFAULT_CONFIG();

	ESP_ERROR_CHECK(httpd_start(&http_server, &http_config));

	httpd_uri_t homepage_uri = {
		.uri = "/",
		.method = HTTP_GET,
		.handler = homepage_uri_handler,
		.user_ctx = NULL,
	};
}

esp_err_t homepage_uri_handler(void){

}
