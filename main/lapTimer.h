/*
 * lapTimer.h
 *
 *  Created on: 31 May 2023
 *      Author: mirek
 */

#ifndef MAIN_LAPTIMER_H_
#define MAIN_LAPTIMER_H_

#include "freertos/freeRTOS.h"
#include "freertos/task.h"

#include "lapTimer_types.h"
#include "lapTimer_wifi.h"

extern State _state;

void state_machine(void);
#endif /* MAIN_LAPTIMER_H_ */
