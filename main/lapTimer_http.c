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

static const httpd_uri_t root_uri = {
		.uri = "/",
		.method = HTTP_GET,
		.handler = root_uri_handler,
		.user_ctx = NULL, // @suppress("Symbol is not resolved")
	};

static const httpd_uri_t cmd_uri = {
		.uri = "/command",
		.method = HTTP_POST,
		.handler = cmd_uri_handler,
		.user_ctx = NULL, // @suppress("Symbol is not resolved")
	};

	static const httpd_uri_t update_uri = {
		.uri="/update",
		.method = HTTP_GET,
		.handler = update_uri_handler,
		.user_ctx = NULL,
	};

	ESP_ERROR_CHECK(httpd_start(&http_server, &http_config));
	ESP_ERROR_CHECK(httpd_register_uri_handler(http_server, &root_uri));
	ESP_ERROR_CHECK(httpd_register_uri_handler(http_server, &cmd_uri));
	ESP_ERROR_CHECK(httpd_register_uri_handler(http_server, &update_uri));
}

static esp_err_t root_uri_handler(httpd_req_t* request){
	const char* response = "<!DOCTYPE html>\n<html lang=\"en\">\n\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>AGH Racing lap timer</title>\n    <style>\n        body {\n            background: rgb(0, 115, 29);\n            background: linear-gradient(135deg, rgba(0, 115, 29, 1) 0%, rgba(0, 44, 11, 1) 20%, rgba(0, 0, 0, 1) 37%, rgba(4, 0, 0, 1) 63%, rgba(50, 0, 0, 1) 73%, rgba(156, 0, 0, 1) 100%);\n        }\n\n        #test {\n            height: 3em;\n        }\n    </style>\n</head>\n\n<body>\n    <button id=\"test\">press me</button>\n    <script type=\"text/javascript\">\n        document.querySelector(\'#test\').addEventListener(\'click\', async () => {\n            resp = await fetch(\'/cmd\', { method: \"POST\", });\n            john = await resp.json();\n            console.log(resp);\n        })\n    </script>\n</body>\n\n</html>";
	httpd_resp_send(request, response, strlen(response));

	return ESP_OK;
}

static esp_err_t cmd_uri_handler(httpd_req_t* request){
	ESP_LOGI("HTTP", "klyk");
	httpd_resp_send(request, "", 0);
	return ESP_OK;
}

static esp_err_t update_uri_handler(httpd_req_t* request){
	return ESP_OK;
}