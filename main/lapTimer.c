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
      i2c_initialize();
      wifi_initialize();
      break;
    case MASTER_INIT:
      start_ap();
      break;
    case MASTER:
      i2c_read();
      break;
    case SLAVE_INIT:
      break;
    case SLAVE:
      break;
    case ERROR:
      break;
  }
}
