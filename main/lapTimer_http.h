/*
 * lapTimer_http.h
 *
 *  Created on: 8 cze 2023
 *      Author: mirek
 */

#ifndef MAIN_LAPTIMER_HTTP_H_
#define MAIN_LAPTIMER_HTTP_H_

#include "esp_http_server.h"
#include "esp_log.h"
#include "lapTimer_types.h"
#include "lapTimer_splits.h"
#include "lapTimer_status.h"

extern httpd_handle_t http_server;
extern SplitGroup_t splits;

void http_server_initialize(void);
static esp_err_t root_uri_handler(httpd_req_t*);
static esp_err_t cmd_uri_handler(httpd_req_t*);
static esp_err_t update_uri_handler(httpd_req_t*);
void kill_http(void);

#endif /* MAIN_LAPTIMER_HTTP_H_ */
