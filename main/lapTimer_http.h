/*
 * lapTimer_http.h
 *
 *  Created on: 8 cze 2023
 *      Author: mirek
 */

#ifndef MAIN_LAPTIMER_HTTP_H_
#define MAIN_LAPTIMER_HTTP_H_

#include "esp_http_server.h"

void http_server_initialize(void);
esp_err_t homepage_uri_handler(void);

#endif /* MAIN_LAPTIMER_HTTP_H_ */
