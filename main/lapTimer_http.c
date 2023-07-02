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

static const httpd_uri_t root_uri = {
		.uri = "/",
		.method = HTTP_GET,
		.handler = root_uri_handler,
		.user_ctx = NULL, // @suppress("Symbol is not resolved")
	};

	ESP_ERROR_CHECK(httpd_start(&http_server, &http_config));
	ESP_ERROR_CHECK(httpd_register_uri_handler(http_server, &root_uri));
}

static esp_err_t root_uri_handler(httpd_req_t* request){
	const char* response = "Siema from nojo";
	httpd_resp_send(request, response, strlen(response));

	return ESP_OK;
}
