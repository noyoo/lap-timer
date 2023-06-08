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
      wifi_initialize();
      break;
    case MASTER_INIT:
      start_ap();
      break;
    case MASTER:
      break;
    case SLAVE_INIT:
      break;
    case SLAVE:
      break;
    case ERROR:
      break;
  }
}
